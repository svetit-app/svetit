import { Component, OnInit, ViewChild, Inject, Injectable } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator, PageEvent } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { Space } from '../model';
import { SpaceInvitation } from '../model';
import { SpaceLink } from '../model';
import { SpaceUser } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { UserFields } from '../../user/model';

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
	currentSpaceId;

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
		if (pageSize?.invitations && pageSize?.links && pageSize?.users){
			this.pageSize = pageSize;
		}

		this.currentSpaceId = this.route.snapshot.paramMap.get('id');
		this.getCurrentSpace();
		this.getInvitations(this.pageSize.invitations, 0);
		this.getLinks(this.pageSize.links, 0);
		this.getUsers(this.pageSize.users, 0);
	}

	getCurrentSpace() {
		this.space.getById(this.currentSpaceId)
			.subscribe(res => {
				this.currentSpace = res;
			});
	}

	getInvitations(limit: number, page: number) {
		this.savePageSize("invitations", limit);
		this.space.getInvitationListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.invitations = res.results as SpaceInvitationDetail[];
				this.invitationsTotal = res.count;
				this.user.fillUserFields(this.invitations);
			});
	}

	getLinks(limit: number, page: number) {
		this.savePageSize("links", limit);
		this.space.getLinkListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.links = res.results;
				this.linksTotal = res.count;
			});
	}

	getUsers(limit: number, page: number) {
		this.savePageSize("users", limit);
		this.space.getUserListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.users = res.results as SpaceUserDetail[];
				this.usersTotal = res.count;
				this.user.fillUserFields(this.users);
			});
	}

	onLinkCopyBtn(link: SpaceLink){
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

	onInvitationDelBtn(invitation: SpaceInvitation){
		this.space.delInvitationById(invitation.id)
			.subscribe(res => {
				if (this.invitationsPaginator.pageIndex == 0) {
					this.getInvitations(this.pageSize.invitations, 0);
				} else {
					this.invitationsPaginator.firstPage();
				}
			}, err => console.warn(err));
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id)
			.subscribe(res => {
				if (this.linksPaginator.pageIndex == 0) {
					this.getLinks(this.pageSize.links, 0);
				} else {
					this.linksPaginator.firstPage();
				}
			}, err => console.warn(err));
	}

	onUserDelBtn(user: SpaceUser){
		this.space.delUserById(user.userId)
			.subscribe(res => {
				if (this.usersPaginator) {
					this.getUsers(this.pageSize.users, 0);
				} else {
					this.usersPaginator.firstPage();
				}
			}, err => console.warn(err));
	}

	private _initInvitationForm() {
		this.invitationForm = this.fb.group({
			login: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
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

	onSubmitInvitation(): void {
		if (this.invitationForm.invalid) {
			return;
		}
		let userId;
		this.user.getByLogin(this.invitationForm.value.login)
		.subscribe(res => {
			userId = res.id;
			this.space.createInvitation(
				this.currentSpaceId,
				userId,
				this.invitationForm.value.role,
				this.currentUserId
			).subscribe(res => {
				this.invitationForm.reset();
				this.isInvitationFormHidden = true;
				if (this.invitationsPaginator.pageIndex == 0) {
					this.getInvitations(this.pageSize.invitations, 0);
				} else {
					this.invitationsPaginator.firstPage();
				}
			});
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
			this.currentSpaceId,
			this.currentUserId,
			this.linkForm.value.name,
			this.linkForm.value.expiredAt
		).subscribe(res => {
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
