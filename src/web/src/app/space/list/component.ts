import { Component, OnInit, Inject, Injectable, ViewChild, ElementRef } from '@angular/core';
import { NgFor, DOCUMENT } from '@angular/common';
import { MatPaginator} from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { Observable} from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';

import { Space, SpaceInvitation, SpaceLink, SpaceFields} from '../model';
import { UserFields } from '../../user/model';
import { User } from '../../user/model';
import { SpaceService } from '../service';
import { UserService } from '../../user/service';

type SpaceInvitationDetail = SpaceInvitation & SpaceFields & UserFields;
type SpaceLinkDetail = SpaceLink & SpaceFields;

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {

	invitationForm: FormGroup;
	isInvitationFormHidden: boolean = true;
	invitationFormSpaceId: string;
	invitationFormSpaceName: string;

	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;
	linkFormSpaceId: string;
	linkFormSpaceName: string;

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
	
	invitations: SpaceInvitationDetail[] = [];
	links: SpaceLinkDetail[] = [];
	spaces: Space[] = [];

	users$: Observable<User[]>;

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

		const pageSizeStr = localStorage.getItem('spaceListPageSize');
		const pageSize = JSON.parse(pageSizeStr);
		if (pageSize?.invitations && pageSize?.links && pageSize?.spaces){
			this.pageSize = pageSize;
		}

		this.getInvitations(this.pageSize.invitations, 0);
		this.getLinks(this.pageSize.links, 0);
		this.getSpaces(this.pageSize.spaces, 0);

		this.users$ = this.invitationForm.controls['login'].valueChanges.pipe(
			startWith(''),
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value => {
				return this.user.getList(10, 0, value || '').pipe(
					map(res => res.results)
				);	
			})
		);
	}

	getSpaces(limit: number, page: number) {
		this.savePageSize("spaces", limit);
		this.space.getList(limit, page)
			.subscribe(res => {
				this.spaces = res.results;
				this.spacesTotal = res.count;
			});
	}

	getInvitations(limit: number, page: number) {
		this.savePageSize("invitations", limit);
		this.space.getInvitationList(limit, page)
			.subscribe(res => {
				this.invitations = res.results as SpaceInvitationDetail[];
				this.invitationsTotal = res.count;
				this.space.fillFields(this.invitations);
				this.user.fillFields(this.invitations);
			});
	}

	getLinks(limit: number, page: number) {
		this.savePageSize("links", limit);
		this.space.getLinkList(limit, page)
			.subscribe(res => {
				this.links = res.results as SpaceLinkDetail[];
				this.linksTotal = res.count;
				this.space.fillFields(this.links);
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
				if (this.invitationsPaginator.pageIndex == 0){
					this.getInvitations(this.pageSize.invitations, 0);
				} else {
					this.invitationsPaginator.firstPage();
				}
			}, err => console.warn(err));
	}

	onLinkDelBtn(link: SpaceLink){
		this.space.delLinkById(link.id)
			.subscribe(res => {
				if (this.linksPaginator.pageIndex == 0){
					this.getLinks(this.pageSize.links, 0);
				} else {
					this.linksPaginator.firstPage();
				}			
			});
	}

	onSpaceDelBtn(space: Space){
		this.space.delSpaceById(space.id)
			.subscribe(res => {
				if (this.spacesPaginator.pageIndex == 0) {
					this.getSpaces(this.pageSize.spaces, 0);
				} else {
					this.spacesPaginator.firstPage();
				}
			});
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
		this.space.getById(space.id)
			.subscribe(res => {
				this.invitationFormSpaceName = res.name;
				this.isInvitationFormHidden = false;
				this.invitationFormSpaceId = space.id;
				this.scrollToInvitationForm.nativeElement.scrollIntoView();
			});
	}

	onSpaceAddLink(space: Space) {
		this.space.getById(space.id)
			.subscribe(res => {
				this.isLinkFormHidden = false;
				this.linkFormSpaceId = space.id;
				this.linkFormSpaceName = res.name;
				this.scrollToLinkForm.nativeElement.scrollIntoView();
			});
	}

	onInvitationFormCloseBtn() {
		this.isInvitationFormHidden = true;
		this.invitationFormSpaceName = "";
		this.invitationForm.reset();
	}

	onLinkFormCloseBtn() {
		this.isLinkFormHidden = true;
		this.linkFormSpaceName = "";
		this.linkForm.reset();
	}

	onSubmitInvitation(): void {
		if (this.invitationForm.invalid) {
			return;
		}

		this.user.getByLogin(this.invitationForm.value.login)
			.subscribe(user => {
				this.space.createInvitation(
					this.invitationFormSpaceId,
					user.id,
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

	onSubmitLink(): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.space.createLink(
			this.linkFormSpaceId,
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
		localStorage.setItem('spaceListPageSize', JSON.stringify(this.pageSize));
	}
}
