import { Component, OnInit, Inject, Injectable, ViewChild, ElementRef } from '@angular/core';
import { NgFor, DOCUMENT } from '@angular/common';
import { MatPaginator, PageEvent } from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { Space } from '../model';
import { SpaceInvitation } from '../model';
import { SpaceLink } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {

	inviteForm: FormGroup;
	isInviteFormHidden: boolean = true;
	inviteFormSpaceId: string;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;
	linkFormSpaceId: string;

	invitesPageIndex: number = 0;
	invitesPageSize: number = 7;

	linksPageIndex: number = 0;
	linksPageSize: number = 7;

	spacesPageIndex: number = 0;
	spacesPageSize: number = 7;

	// userId текущего залогиненного юзера
	currentUserId: string = "2";

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/space/link/";

	spacesTotal: number;
	invitesTotal: number;
	linksTotal: number;
	
	invites: SpaceInvitation[] = [];
	links: SpaceLink[] = [];
	spaces: Space[] = [];

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;
	@ViewChild('scrollToInviteForm') scrollToInviteForm: ElementRef;
	@ViewChild('scrollToLinkForm') scrollToLinkForm: ElementRef;

	constructor(
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._initInviteForm();
		this._initLinkForm();
	}

	ngOnInit() {
		let pageSize = localStorage.getItem('invitesPaginatorSpaceListPageSize');
		this.invitesPageSize = pageSize ? parseInt(pageSize) : 7;
		
		pageSize = localStorage.getItem('linksPaginatorSpaceListPageSize');
		this.linksPageSize = pageSize ? parseInt(pageSize) : 7;

		pageSize = localStorage.getItem('spacesPaginatorSpaceListPageSize');
		this.spacesPageSize = pageSize ? parseInt(pageSize) : 7;

		this.getInvites(this.invitesPageSize, this.invitesPageIndex);
		this.getLinks(this.linksPageSize, this.linksPageIndex);
		this.getSpaces(this.spacesPageSize, this.spacesPageIndex);
	}

	getSpaces(limit: number, page: number) {
		this.space.getSpaceList(limit, page)
			.subscribe(res => {
				this.spaces = res.results;
				this.spacesTotal = res.count;
			});
	}

	getInvites(limit: number, page: number) {
		this.space.getInvitesList(limit, page)
			.subscribe(res => {
				this.invites = res.results;
				this.invitesTotal = res.count;
			});
	}

	getLinks(limit: number, page: number) {
		this.space.getLinksList(limit, page)
			.subscribe(res => {
				this.links = res.results;
				this.linksTotal = res.count;
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

	onSpaceDelBtn(space: Space){
		this.space.delSpaceById(space.id);
		this.spacesPageIndex = 0;
		if (this.spacesPaginator) {
			this.spacesPaginator.firstPage();
		}
		this.getSpaces(this.spacesPageSize, this.spacesPageIndex);
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

	private _initLinkForm() {
		this.linkForm = this.fb.group({
			name: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			expiredAt: [null, [Validators.required]],
		});
	}

	onSpaceInviteAddUser(space: Space) {
		this.isInviteFormHidden = false;
		this.inviteFormSpaceId = space.id;
		this.scrollToInviteForm.nativeElement.scrollIntoView();
	}

	onSpaceAddLink(space: Space) {
		this.isLinkFormHidden = false;
		this.linkFormSpaceId = space.id;
		this.scrollToLinkForm.nativeElement.scrollIntoView();
	}

	onInviteFormCloseBtn() {
		this.isInviteFormHidden = true;
		this.inviteForm.reset();
	}

	onLinkFormCloseBtn() {
		this.isLinkFormHidden = true;
		this.linkForm.reset();
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
			this.inviteFormSpaceId,
			userId,
			data.value.role,
			this.currentUserId);
			
		this.invitesPageIndex = 0;
		this.inviteForm.reset();
		this.isInviteFormHidden = true;
		if (this.invitesPaginator) {
			this.invitesPaginator.firstPage();
		}
		this.getInvites(this.invitesPageSize, this.invitesPageIndex);
	}

	onSubmitLink(data): void {
		if (this.linkForm.invalid) {
			return;
		}
	
		this.space.createNewLink(
			this.linkFormSpaceId,
			this.currentUserId,
			data.value.name,
			data.value.expiredAt);

		this.linksPageIndex = 0;
		this.linkForm.reset();
		this.isLinkFormHidden = true;
		if (this.linksPaginator) {
			this.linksPaginator.firstPage();
		}
		this.getLinks(this.linksPageSize, this.linksPageIndex);
	}

	getSpaceNameById(spaceId: string) {
		let name;
		this.space.getSpaceNameById(spaceId)
			.subscribe(res => {
				name = res;
			});
		return name;
	}
	
	savePageSizeToLocalStorage(id: string, pageEvent: PageEvent) {
		localStorage.setItem(id+'PageSize', pageEvent.pageSize.toString());
	}

	getUsernameById(userId: string) {
		let username;
		this.user.getById(userId)
		.subscribe(res => {
			username = res.username;
		});
		return username;
	}
}
