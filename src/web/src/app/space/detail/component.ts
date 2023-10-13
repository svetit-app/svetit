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
import { RequestWatcherService } from '../../request-watcher/service';

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
	currentUserId: string = "2";
	currentSpaceId;

	linksURL: string = "/space/link/";

	invitations: SpaceInvitation[] = [];
	links: SpaceLink[] = [];
	users: SpaceUser[] = [];

	invitationsPageSize: number = 7;
	linksPageSize: number = 7;
	usersPageSize: number = 10;

	invitationsTotal: number;
	linksTotal: number;
	usersTotal: number;

	@ViewChild('invitationsPaginator') invitationsPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	currentSpaceLoaded = false;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
		public requestWatcher: RequestWatcherService,
	) {
		this._initInvitationForm();
		this._initLinkForm();
	}

	ngOnInit() {
		let pageSize = localStorage.getItem('invitationsPaginatorSpaceDetailPageSize');
		this.invitationsPageSize = pageSize ? parseInt(pageSize) : 7;
		
		pageSize = localStorage.getItem('linksPaginatorSpaceDetailPageSize');
		this.linksPageSize = pageSize ? parseInt(pageSize) : 7;

		pageSize = localStorage.getItem('usersPaginatorSpaceDetailPageSize');
		this.usersPageSize = pageSize ? parseInt(pageSize) : 10;

		this.currentSpaceId = this.route.snapshot.paramMap.get('id');
		this.getCurrentSpace(this.currentSpaceId);
		this.getInvitations(this.invitationsPageSize, 0);
		this.getLinks(this.linksPageSize, 0);
		this.getUsers(this.usersPageSize, 0);
	}

	getCurrentSpace(spaceId: string) {
		this.space.getById(spaceId)
			.subscribe(res => {
				this.currentSpace = res;
				this.currentSpaceLoaded = true;
			});
	}

	getInvitations(limit: number, page: number) {
		this.space.getInvitationListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.invitations = res.results;
				this.invitationsTotal = res.count;
			});
	}

	getLinks(limit: number, page: number) {
		this.space.getLinkListForSpace(this.currentSpaceId, limit, page)
			.subscribe(res => {
				this.links = res.results;
				this.linksTotal = res.count;
			});
	}

	getUsers(limit: number, page: number) {
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
		this.space.delInvitationById(invitation.id).pipe(
			src => this.requestWatcher.WatchFor(src)
		).subscribe(res => {});
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.invitationsPageSize, 0);
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id).pipe(
			src => this.requestWatcher.WatchFor(src)
		).subscribe(res => {});
		this.linksPaginator.firstPage();
		this.getLinks(this.linksPageSize, 0);
	}

	onUserDelBtn(user: SpaceUser){
		this.space.delUserById(user.userId).pipe(
			src => this.requestWatcher.WatchFor(src)
		).subscribe(res => {});
		this.usersPaginator.firstPage();
		this.getUsers(this.usersPageSize, 0);
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
		).pipe(
			src => this.requestWatcher.WatchFor(src)
		).subscribe(res => {});
			
		this.invitationForm.reset();
		this.isInvitationFormHidden = true;
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.invitationsPageSize, 0);
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
		).pipe(
			src => this.requestWatcher.WatchFor(src)
		).subscribe(res => {});

		this.linkForm.reset();
		this.isLinkFormHidden = true;
		this.linksPaginator.firstPage();
		this.getLinks(this.linksPageSize, 0);
	}

	savePageSizeToLocalStorage(id: string, pageEvent: PageEvent) {
		localStorage.setItem(id+'PageSize', pageEvent.pageSize.toString());
		switch(id){
			case "invitationsPaginatorSpaceDetail":
				this.invitationsPageSize = pageEvent.pageSize;
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
