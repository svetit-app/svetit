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

	invitationForm: FormGroup;
	isInvitationFormHidden: boolean = true;
	invitationFormSpaceId: string;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;
	linkFormSpaceId: string;

	pageSize = {
		invitations: 7,
		links: 7,
		spaces: 7
	};

	// userId текущего залогиненного юзера
	currentUserId: string;

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/space/link/";

	spacesTotal: number;
	invitationsTotal: number;
	linksTotal: number;
	
	invitations: SpaceInvitation[] = [];
	links: SpaceLink[] = [];
	spaces: Space[] = [];

	@ViewChild('invitationsPaginator') invitationsPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;
	@ViewChild('scrollToInvitationForm') scrollToInvitationForm: ElementRef;
	@ViewChild('scrollToLinkForm') scrollToLinkForm: ElementRef;

	constructor(
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

		let pageSize = localStorage.getItem('invitationsPaginatorSpaceListPageSize');
		this.pageSize.invitations = pageSize ? parseInt(pageSize) : 7;
		
		pageSize = localStorage.getItem('linksPaginatorSpaceListPageSize');
		this.pageSize.links = pageSize ? parseInt(pageSize) : 7;

		pageSize = localStorage.getItem('spacesPaginatorSpaceListPageSize');
		this.pageSize.spaces = pageSize ? parseInt(pageSize) : 7;

		this.getInvitations(this.pageSize.invitations, 0);
		this.getLinks(this.pageSize.links, 0);
		this.getSpaces(this.pageSize.spaces, 0);
	}

	getSpaces(limit: number, page: number) {
		this.space.getList(limit, page)
			.subscribe(res => {
				this.spaces = res.results;
				this.spacesTotal = res.count;
			});
	}

	getInvitations(limit: number, page: number) {
		this.space.getInvitationList(limit, page)
			.subscribe(res => {
				this.invitations = res.results;
				this.invitationsTotal = res.count;
			});
	}

	getLinks(limit: number, page: number) {
		this.space.getLinkList(limit, page)
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

	onInvitationDelBtn(invitation: SpaceInvitation){
		this.space.delInvitationById(invitation.id)
			.subscribe(res => {});
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.pageSize.invitations, 0);
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id)
			.subscribe(res => {});
		this.linksPaginator.firstPage();
		this.getLinks(this.pageSize.links, 0);
	}

	onSpaceDelBtn(space: Space){
		this.space.delSpaceById(space.id)
			.subscribe(res => {});
		this.spacesPaginator.firstPage();
		this.getSpaces(this.pageSize.spaces, 0);
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

	private _initLinkForm() {
		this.linkForm = this.fb.group({
			name: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			expiredAt: [null, [Validators.required]],
		});
	}

	onSpaceInvitationAddUser(space: Space) {
		this.isInvitationFormHidden = false;
		this.invitationFormSpaceId = space.id;
		this.scrollToInvitationForm.nativeElement.scrollIntoView();
	}

	onSpaceAddLink(space: Space) {
		this.isLinkFormHidden = false;
		this.linkFormSpaceId = space.id;
		this.scrollToLinkForm.nativeElement.scrollIntoView();
	}

	onInvitationFormCloseBtn() {
		this.isInvitationFormHidden = true;
		this.invitationForm.reset();
	}

	onLinkFormCloseBtn() {
		this.isLinkFormHidden = true;
		this.linkForm.reset();
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
			this.invitationFormSpaceId,
			userId,
			data.value.role,
			this.currentUserId
		).subscribe(res => {});
			
		this.invitationForm.reset();
		this.isInvitationFormHidden = true;
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.pageSize.invitations, 0);
	}

	onSubmitLink(data): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.space.createLink(
			this.linkFormSpaceId,
			this.currentUserId,
			data.value.name,
			data.value.expiredAt
		).subscribe(res => {});

		this.linkForm.reset();
		this.isLinkFormHidden = true;
		this.linksPaginator.firstPage();
		this.getLinks(this.pageSize.links, 0);
	}

	getSpaceNameById(spaceId: string) {
		let space;
		this.space.getById(spaceId)
			.subscribe(res => {
				space = res;
			});
		return space?.name;
	}
	
	savePageSizeToLocalStorage(id: string, pageEvent: PageEvent) {
		localStorage.setItem(id+'PageSize', pageEvent.pageSize.toString());
		switch(id){
			case "invitationsPaginatorSpaceList":
				this.pageSize.invitations = pageEvent.pageSize;
				break;
			case "linksPaginatorSpaceList":
				this.pageSize.links = pageEvent.pageSize;
				break;
			case "spacesPaginatorSpaceList":
				this.pageSize.spaces = pageEvent.pageSize;
				break;
		}
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
