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

	inviteForm: FormGroup;
	isInviteFormHidden: boolean = true;
	inviteFormSpaceId: string;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;
	linkFormSpaceId: string;

	invitesPageSize: number = 7;
	linksPageSize: number = 7;
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

	displayProgressSpinner = false;

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

		this.getInvites(this.invitesPageSize, 0);
		this.getLinks(this.linksPageSize, 0);
		this.getSpaces(this.spacesPageSize, 0);
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
		this.showProgressSpinner();
		this.space.delInviteById(invite.id).subscribe(res => {
			if (res != true) {
				alert("error!");
			}
			this.hideProgressSpinner();
		});
		this.invitesPaginator.firstPage();
		this.getInvites(this.invitesPageSize, 0);
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
		this.showProgressSpinner();
		let userId;
		this.user.getByUsername(data.value.username)
			.subscribe(res => {
				userId = res.id;
			});

		this.space.createInvite(
			this.inviteFormSpaceId,
			userId,
			data.value.role,
			this.currentUserId
		).subscribe(res => {
			if (res != true){
				alert("error!");
			}
			this.hideProgressSpinner();
		});
			
		this.inviteForm.reset();
		this.isInviteFormHidden = true;
		this.invitesPaginator.firstPage();
		this.getInvites(this.invitesPageSize, 0);
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
			case "invitesPaginatorSpaceList":
				this.invitesPageSize = pageEvent.pageSize;
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
