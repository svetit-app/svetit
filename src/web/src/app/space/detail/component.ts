import { Component, OnInit, ViewChild, Inject} from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';
import { FormGroup, FormBuilder, Validators} from '@angular/forms';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';
import { Observable } from 'rxjs';
import { MatOption } from '@angular/material/core';

import { Space, SpaceInvitation, SpaceLink, SpaceUser } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { User, UserFields } from '../../user/model';

type SpaceInvitationDetail = SpaceInvitation & UserFields;
type SpaceUserDetail = SpaceUser & UserFields;

@Component({
	selector: 'app-space-detail',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceDetailComponent implements OnInit {
	invitationForm: FormGroup;
	isInvitationFormHidden: boolean = true;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;

	currentSpace: Space;
	currentUserId: string;

	linksURL: string = "/space/link/";

	invitations: SpaceInvitationDetail[] = [];
	links: SpaceLink[] = [];
	users: SpaceUserDetail[] = [];

	pageSize = {
		invitations: 7,
		links: 7,
		users: 10
	};

	invitationsTotal: number;
	linksTotal: number;
	usersTotal: number;

	users$: Observable<User[]>;
	selectedUser: User;

	@ViewChild('invitationsPaginator') invitationsPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._initInvitationForm();
		this._initLinkForm();
	}

	ngOnInit() {
		this.currentUserId = this.user.info.id;

		const pageSizeStr = localStorage.getItem('spaceDetailPageSize');
		const pageSize = JSON.parse(pageSizeStr);
		if (pageSize?.invitations && pageSize?.links && pageSize?.users) {
			this.pageSize = pageSize;
		}

		const key = this.route.snapshot.paramMap.get('key');
		this.getInitData(key);

		this.users$ = this.invitationForm.controls['login'].valueChanges.pipe(
			startWith(''),
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value => this.user.getList(10, 0, value || '').pipe(
				map(res => res.results)
			))
		);
	}

	getInitData(key: string) {
		this.space.getByKey(key)
			.subscribe(res => {
				this.currentSpace = res;
				this.getInvitations(this.pageSize.invitations, 0);
				this.getLinks(this.pageSize.links, 0);
				this.getUsers(this.pageSize.users, 0);
		});
	}

	getInvitations(limit: number, page: number) {
		this.savePageSize("invitations", limit);
		this.space.getInvitationListForSpace(this.currentSpace.id, limit, page)
			.subscribe(res => {
				this.invitations = res.results as SpaceInvitationDetail[];
				this.invitationsTotal = res.count;
				this.user.fillFields(this.invitations);
			});
	}

	getLinks(limit: number, page: number) {
		this.savePageSize("links", limit);
		this.space.getLinkListForSpace(this.currentSpace.id, limit, page)
			.subscribe(res => {
				this.links = res.results;
				this.linksTotal = res.count;
			});
	}

	getUsers(limit: number, page: number) {
		this.savePageSize("users", limit);
		this.space.getUserListForSpace(this.currentSpace.id, limit, page)
			.subscribe(res => {
				this.users = res.results as SpaceUserDetail[];
				this.usersTotal = res.count;
				this.user.fillFields(this.users);
			});
	}

	onLinkCopyBtn(link: SpaceLink) {
		let copyToClipboard = this.document.createElement('textarea');
		copyToClipboard.style.position = 'fixed';
		copyToClipboard.style.left = '0';
		copyToClipboard.style.top = '0';
		copyToClipboard.style.opacity = '0';
		copyToClipboard.value = this.document.location.origin + this.linksURL + link.id;
		document.body.appendChild(copyToClipboard);
		copyToClipboard.focus();
		copyToClipboard.select();
		document.execCommand('copy');
		document.body.removeChild(copyToClipboard);
	}

	onInvitationDelBtn(invitation: SpaceInvitation) {
		this.space.delInvitationById(invitation.id)
			.subscribe(_ => {
				if (this.invitationsPaginator.pageIndex == 0) {
					this.getInvitations(this.pageSize.invitations, 0);
				} else {
					this.invitationsPaginator.firstPage();
				}
			});
	}

	onLinkDelBtn(link: SpaceLink) {
		this.space.delLinkById(link.id)
			.subscribe(_ => {
				if (this.linksPaginator.pageIndex == 0) {
					this.getLinks(this.pageSize.links, 0);
				} else {
					this.linksPaginator.firstPage();
				}
			});
	}

	onUserDelBtn(user: SpaceUser) {
		this.space.delUserById(user.userId)
			.subscribe(_ => {
				if (this.usersPaginator.pageIndex == 0) {
					this.getUsers(this.pageSize.users, 0);
				} else {
					this.usersPaginator.firstPage();
				}
			});
	}

	private _initInvitationForm() {
		this.invitationForm = this.fb.group({
			login: ['', [
				Validators.required
			]],
			role: ['', [
				Validators.required
			],],
		});
	}

	onInvitationAdd() {
		this.isInvitationFormHidden = false;
	}

	onInvitationFormCloseBtn() {
		this.isInvitationFormHidden = true;
		this.invitationForm.reset();
	}

	onSelectUser(option: MatOption) {
		if (option?.value) {
			console.log(option?.value);
			this.selectedUser = option.value;
		}
	}

	displayUserLogin(value) {
		return value?.login;
	}

	onSubmitInvitation(): void {
		if (this.invitationForm.invalid) {
			return;
		}
		this.space.createInvitation(
			this.currentSpace.id,
			this.selectedUser.id,
			this.invitationForm.value.role,
			this.currentUserId
		).subscribe(_ => {
			this.invitationForm.reset();
			this.isInvitationFormHidden = true;
			if (this.invitationsPaginator.pageIndex == 0) {
				this.getInvitations(this.pageSize.invitations, 0);
			} else {
				this.invitationsPaginator.firstPage();
			}
		});
	}

	private _initLinkForm() {
		this.linkForm = this.fb.group({
			name: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			expiredAt: [null, [Validators.required]],
		});
	}

	onLinkAdd() {
		this.isLinkFormHidden = false;
	}

	onLinkFormCloseBtn() {
		this.isLinkFormHidden = true;
		this.linkForm.reset();
	}

	onSubmitLink(): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.space.createLink(
			this.currentSpace.id,
			this.currentUserId,
			this.linkForm.value.name,
			this.linkForm.value.expiredAt
		).subscribe(_ => {
			this.linkForm.reset();
			this.isLinkFormHidden = true;
			if (this.linksPaginator.pageIndex == 0) {
				this.getLinks(this.pageSize.links, 0);
			} else {
				this.linksPaginator.firstPage();
			}
		});
	}

	savePageSize(id: string, limit: number) {
		if (this.pageSize[id] == limit)
			return;

		this.pageSize[id] = limit;
		localStorage.setItem('spaceDetailPageSize', JSON.stringify(this.pageSize));
	}
}
