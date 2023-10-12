import { Component, OnInit, Inject, Injectable, ViewChild, ElementRef } from '@angular/core';
import { NgFor, DOCUMENT } from '@angular/common';
import { MatPaginator, PageEvent } from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { Space } from '../model';
import { SpaceInvitation } from '../model';
import { SpaceLink } from '../model';

import { SpaceService } from '../service';
import { UserService } from '../../user/service';

import { OverlayService } from '../../overlay/overlay.service';
import { ProgressSpinnerComponent } from '../../progress-spinner/progress-spinner.component';

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

	invitationsPageSize: number = 7;
	linksPageSize: number = 7;
	spacesPageSize: number = 7;

	// userId текущего залогиненного юзера
	currentUserId: string = "2";

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

	displayProgressSpinner = false;

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
		let pageSize = localStorage.getItem('invitationsPaginatorSpaceListPageSize');
		this.invitationsPageSize = pageSize ? parseInt(pageSize) : 7;
		
		pageSize = localStorage.getItem('linksPaginatorSpaceListPageSize');
		this.linksPageSize = pageSize ? parseInt(pageSize) : 7;

		pageSize = localStorage.getItem('spacesPaginatorSpaceListPageSize');
		this.spacesPageSize = pageSize ? parseInt(pageSize) : 7;

		this.getInvitations(this.invitationsPageSize, 0);
		this.getLinks(this.linksPageSize, 0);
		this.getSpaces(this.spacesPageSize, 0);
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
		this.showProgressSpinner();
		this.space.delInvitationById(invitation.id).subscribe(res => {
			if (res != true) {
				alert("error!");
			}
			this.hideProgressSpinner();
		});
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.invitationsPageSize, 0);
	}

	onLinkDelBtn(link: SpaceLink){
		this.showProgressSpinner();
		this.space.delLinkById(link.id).subscribe(res => {
			if (res != true) {
				alert("error!");
			}
			this.hideProgressSpinner();
		});
		this.linksPaginator.firstPage();
		this.getLinks(this.linksPageSize, 0);
	}

	onSpaceDelBtn(space: Space){
		this.showProgressSpinner();
		this.space.delSpaceById(space.id).subscribe(res => {
			if (res != true) {
				alert("error!");
			}
			this.hideProgressSpinner();
		});
		this.spacesPaginator.firstPage();
		this.getSpaces(this.spacesPageSize, 0);
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
		this.showProgressSpinner();
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
		).subscribe(res => {
			if (res != true){
				alert("error!");
			}
			this.hideProgressSpinner();
		});
			
		this.invitationForm.reset();
		this.isInvitationFormHidden = true;
		this.invitationsPaginator.firstPage();
		this.getInvitations(this.invitationsPageSize, 0);
	}

	onSubmitLink(data): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.showProgressSpinner();
		this.space.createLink(
			this.linkFormSpaceId,
			this.currentUserId,
			data.value.name,
			data.value.expiredAt
		).subscribe(res => {
			if (res != true){
				alert("error!");
			}
			this.hideProgressSpinner();
		});

		this.linkForm.reset();
		this.isLinkFormHidden = true;
		this.showProgressSpinner();
		this.linksPaginator.firstPage();
		this.getLinks(this.linksPageSize, 0);
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
		switch(id){
			case "invitationsPaginatorSpaceList":
				this.invitationsPageSize = pageEvent.pageSize;
				break;
			case "linksPaginatorSpaceList":
				this.linksPageSize = pageEvent.pageSize;
				break;
			case "spacesPaginatorSpaceList":
				this.spacesPageSize = pageEvent.pageSize;
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

	showProgressSpinner() {
		this.displayProgressSpinner = true;
	};

	hideProgressSpinner() {
		this.displayProgressSpinner = false;
	};
}
