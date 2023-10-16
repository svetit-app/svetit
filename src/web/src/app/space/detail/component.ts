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

	invitations: SpaceInvitation[] = [];
	links: SpaceLink[] = [];
	users: SpaceUser[] = [];

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
				this.invitations = res.results;
				this.invitationsTotal = res.count;
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
				this.users = res.results;
				this.usersTotal = res.count;
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
				this.invitationsPaginator.firstPage();
				this.getInvitations(this.pageSize.invitations, 0);
			});
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id)
			.subscribe(res => {
				this.linksPaginator.firstPage();
				this.getLinks(this.pageSize.links, 0);
			});
	}

	onUserDelBtn(user: SpaceUser){
		this.space.delUserById(user.userId)
			.subscribe(res => {
				this.usersPaginator.firstPage();
				this.getUsers(this.pageSize.users, 0);
			});
	}

	getUserNameById(userId: string) {
		let name;
		this.user.getById(userId)
		.subscribe(res => {
			name = res.name;
		});
		return name;
	}

	getUserEmailById(userId: string) {
		let email;
		this.user.getById(userId)
		.subscribe(res => {
			email = res.email;
		});
		return email;
	}

	getUsernameById(userId: string) {
		let username;
		this.user.getById(userId)
		.subscribe(res => {
			username = res.username;
		});
		return username;
	}

	private _initInvitationForm() {
		this.invitationForm = this.fb.group({
			username: ['', [
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

	onSubmitInvitation(data): void {
		if (this.invitationForm.invalid) {
			return;
		}
		let userId;
		this.user.getByUsername(data.value.username)
		.subscribe(res => {
			userId = res.id;
		});
	
		this.space.createInvitation(
			this.currentSpaceId,
			userId,
			data.value.role,
			this.currentUserId
		).subscribe(res => {
			this.invitationForm.reset();
			this.isInvitationFormHidden = true;
			this.invitationsPaginator.firstPage();
			this.getInvitations(this.pageSize.invitations, 0);
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

	onSubmitLink(data): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.space.createLink(
			this.currentSpaceId,
			this.currentUserId,
			data.value.name,
			data.value.expiredAt
		).subscribe(res => {
			this.linkForm.reset();
			this.isLinkFormHidden = true;
			this.linksPaginator.firstPage();
			this.getLinks(this.pageSize.links, 0);
		});
	}

	savePageSize(id: string, limit: number) {
		if (this.pageSize[id] == limit)
			return;
		
		this.pageSize[id] = limit;
		localStorage.setItem('spaceDetailPageSize', JSON.stringify(this.pageSize));
	}
}
