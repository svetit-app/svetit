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

	inviteForm: FormGroup;
	isInviteFormHidden: boolean = true;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;

	currentSpace: Space;
	currentUserId: string = "2";
	currentSpaceId;

	linksURL: string = "/space/link/";

	invites: SpaceInvitation[] = [];
	links: SpaceLink[] = [];
	users: SpaceUser[] = [];

	invitesPageIndex: number = 0;
	invitesPageSize: number = 7;

	linksPageIndex: number = 0;
	linksPageSize: number = 7;

	usersPageIndex: number = 0;
	usersPageSize: number = 10;

	invitesTotal: number;
	linksTotal: number;
	usersTotal: number;

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._initInviteForm();
		this._initLinkForm();
	}

	ngOnInit() {
		let pageSize = localStorage.getItem('invitesPaginatorSpaceDetailPageSize');
		this.invitesPageSize = pageSize ? parseInt(pageSize) : 7;
		
		pageSize = localStorage.getItem('linksPaginatorSpaceDetailPageSize');
		this.linksPageSize = pageSize ? parseInt(pageSize) : 7;

		pageSize = localStorage.getItem('usersPaginatorSpaceDetailPageSize');
		this.usersPageSize = pageSize ? parseInt(pageSize) : 10;

		this.currentSpaceId = this.route.snapshot.paramMap.get('id');
		this.getCurrentSpace(this.currentSpaceId);
		this.getInvites(this.invitesPageSize, this.invitesPageIndex);
		this.getLinks(this.linksPageSize, this.linksPageIndex);
		this.getUsers(this.usersPageSize, this.usersPageIndex);
	}

	getCurrentSpace(spaceId: string) {
		this.space.getSpaceById(spaceId)
			.subscribe(res => {
				this.currentSpace = res;
			});
	}

	getInvites(limit: number, page: number) {
		this.space.getInvitesListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.invites = res.results;
				this.invitesTotal = res.count;
			});
	}

	getLinks(limit: number, page: number) {
		this.space.getLinksListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.links = res.results;
				this.linksTotal = res.count;
			});
	}

	getUsers(limit: number, page: number) {
		this.space.getUsersListForSpace(this.currentSpaceId, limit, page)
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

	onInviteDelBtn(invite: SpaceInvitation){
		this.space.delInviteById(invite.id);
		this.invitesPageIndex = 0;
		if (this.invitesPaginator) {
			this.invitesPaginator.firstPage();
		}
		this.getInvites(this.invitesPageSize, this.invitesPageIndex);
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id);
		this.linksPageIndex = 0;
		if (this.linksPaginator) {
			this.linksPaginator.firstPage();
		}
		this.getLinks(this.linksPageSize, this.linksPageIndex);
	}

	onUserDelBtn(user: SpaceUser){
		this.space.delUserById(user.userId);
		this.usersPageIndex = 0;
		if (this.usersPaginator) {
			this.usersPaginator.firstPage();
		}
		this.getUsers(this.usersPageSize, this.usersPageIndex);
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

	private _initInviteForm() {
		this.inviteForm = this.fb.group({
			username: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			role: ['', [
				Validators.required
			],],
		});
	}
	
	onInviteAdd() {
		this.isInviteFormHidden = false;
	}

	onInviteFormCloseBtn() {
		this.isInviteFormHidden = true;
		this.inviteForm.reset();
	}

	onSubmitInvite(data): void {
		if (this.inviteForm.invalid) {
			return;
		}

		let userId;
		this.user.getByUsername(data.value.username)
		.subscribe(res => {
			userId = res.id;
		});
	
		this.space.createNewInvite(
			this.currentSpaceId,
			userId,
			data.value.role,
			this.currentUserId);
			
		this.invitesPageIndex = 0;
		this.inviteForm.reset();
		this.isInviteFormHidden = true;
		this.invitesPaginator.firstPage();
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
	
		this.space.createNewLink(
			this.currentSpaceId,
			this.currentUserId,
			data.value.name,
			data.value.expiredAt);

		this.linksPageIndex = 0;
		this.linkForm.reset();
		this.isLinkFormHidden = true;
		this.linksPaginator.firstPage();
	}

	savePageSizeToLocalStorage(id: string, pageEvent: PageEvent) {
		localStorage.setItem(id+'PageSize', pageEvent.pageSize.toString());
		switch(id){
			case "invitesPaginatorSpaceDetail":
				this.invitesPageSize = pageEvent.pageSize;
				break;
			case "linksPaginatorSpaceDetail":
				this.linksPageSize = pageEvent.pageSize;
				break;
			case "usersPaginatorSpaceDetail":
				this.usersPageSize = pageEvent.pageSize;
				break;
		}
	}
}
