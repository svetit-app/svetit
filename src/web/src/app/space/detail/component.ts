import { Component, OnInit, ViewChild, Inject} from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';
import { FormGroup, FormBuilder, Validators} from '@angular/forms';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';
import { Observable } from 'rxjs';
import { MatOption } from '@angular/material/core';

import { Space, SpaceLink, SpaceUser } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { User, UserFields } from '../../user/model';

type SpaceUserDetail = SpaceUser & UserFields;

@Component({
	selector: 'app-space-detail',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceDetailComponent implements OnInit {
	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;

	currentSpace: Space = {} as Space;
	currentUserId: string;

	linksURL: string = "/space/link/";

	links: SpaceLink[] = [];
	users: SpaceUserDetail[] = [];

	pageSize = {
		invitations: 7,
		links: 7,
		users: 10
	};

	linksTotal: number;
	usersTotal: number;

	selectedUser: User;

	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
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
	}

	getInitData(key: string) {
		this.space.getByKey(key)
			.subscribe(res => {
				this.currentSpace = res;
				this.getLinks(this.pageSize.links, 0);
				this.getUsers(this.pageSize.users, 0);
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
