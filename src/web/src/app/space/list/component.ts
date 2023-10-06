import { Component, OnInit, Inject, Injectable, ViewChild, ElementRef } from '@angular/core';
import { NgFor, DOCUMENT } from '@angular/common';
import { MatPaginator } from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { SpaceInterface } from '../model';
import { SpaceInvitation } from '../model';
import { SpaceLink } from '../model';

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
    invitesLowValue: number = 0;
    invitesHighValue: number = 7;  

	linksPageIndex: number = 0;
    linksPageSize: number = 7;
    linksLowValue: number = 0;
    linksHighValue: number = 7;

	spacesPageIndex: number = 0;
    spacesPageSize: number = 7;
    spacesLowValue: number = 0;
    spacesHighValue: number = 7;

	// userId текущего залогиненного юзера
	currentUser: string = "vasya";

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/links/";

	spaces: SpaceInterface[] = [
		{id: "11111111-1111-1111-1111-111111111111", name: "Пространство №1", key: "key1", requestsAllowed: true, createdAt: new Date("2023-10-01")},
		{id: "22222222-2222-2222-2222-222222222222", name: "Пространство №2", key: "key2", requestsAllowed: true, createdAt: new Date("2023-10-02")},
		{id: "33333333-3333-3333-3333-333333333333", name: "Пространство №3", key: "key3", requestsAllowed: true, createdAt: new Date("2023-10-03")},
		{id: "44444444-4444-4444-4444-444444444444", name: "Пространство №4", key: "key4", requestsAllowed: true, createdAt: new Date("2023-10-04")},
		{id: "55555555-5555-5555-5555-555555555555", name: "Пространство №5", key: "key5", requestsAllowed: true, createdAt: new Date("2023-10-05")},
		{id: "66666666-6666-6666-6666-666666666666", name: "Пространство №6", key: "key6", requestsAllowed: true, createdAt: new Date("2023-10-06")},
		{id: "77777777-7777-7777-7777-777777777777", name: "Пространство №7", key: "key7", requestsAllowed: true, createdAt: new Date("2023-10-07")},
		{id: "88888888-8888-8888-8888-888888888888", name: "Пространство №8", key: "key8", requestsAllowed: true, createdAt: new Date("2023-10-08")},
		{id: "99999999-9999-9999-9999-999999999999", name: "Пространство №9", key: "key9", requestsAllowed: true, createdAt: new Date("2023-10-09")},
	];

	invites: SpaceInvitation[] = [
		// Меня пригласили
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-01")},
		{id: crypto.randomUUID(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-02")},
		// Я прошусь
		{id: crypto.randomUUID(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-03")},
		{id: crypto.randomUUID(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-04")},
		// Мы пригласили
		{id: crypto.randomUUID(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "kolya", role: "user", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-05")},
		{id: crypto.randomUUID(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "petya", role: "guest", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-06")},
		// Хочет к нам
		{id: crypto.randomUUID(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-07")},
		{id: crypto.randomUUID(), spaceId: '88888888-8888-8888-8888-888888888888', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-08")},
		{id: crypto.randomUUID(), spaceId: '99999999-9999-9999-9999-999999999999', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-09")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-10")},
		{id: crypto.randomUUID(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-11")},
		{id: crypto.randomUUID(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-12")},
		{id: crypto.randomUUID(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-13")},
		{id: crypto.randomUUID(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-14")},
		{id: crypto.randomUUID(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-15")},
		{id: crypto.randomUUID(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-16")},
	];

	links: SpaceLink[] = [
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link1", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '22222222-2222-2222-2222-222222222222', creatorId: "vasya", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '33333333-3333-3333-3333-333333333333', creatorId: "vasya", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '44444444-4444-4444-4444-444444444444', creatorId: "vasya", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '55555555-5555-5555-5555-555555555555', creatorId: "vasya", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '66666666-6666-6666-6666-666666666666', creatorId: "vasya", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '77777777-7777-7777-7777-777777777777', creatorId: "vasya", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '88888888-8888-8888-8888-888888888888', creatorId: "vasya", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	];

	receivedInvites: SpaceInvitation[] = [];
	receivedLinks: SpaceLink[] = [];
	receivedSpaces: SpaceInterface[] = [];

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;
	@ViewChild('scrollToInviteForm') scrollToInviteForm: ElementRef;
	@ViewChild('scrollToLinkForm') scrollToLinkForm: ElementRef;

	constructor(
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
	) {
		this._initInviteForm();
		this._initLinkForm();
	}

	ngOnInit() {
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
		this.getLinks(this.linksLowValue, this.linksHighValue);
		this.getSpaces(this.spacesLowValue, this.spacesHighValue);
	}

	getInvites(lowValue, highValue) {
		this.receivedInvites = this.invites.slice(lowValue, highValue);
	}

	getPaginatorInvites(event) {
		if(event.pageIndex === this.invitesPageIndex + 1) {
			this.invitesLowValue = this.invitesLowValue + this.invitesPageSize;
		   	this.invitesHighValue =  this.invitesHighValue + this.invitesPageSize;
		} else if(event.pageIndex === this.invitesPageIndex - 1) {
			this.invitesLowValue= this.invitesLowValue - this.invitesPageSize;
			this.invitesHighValue =  this.invitesHighValue - this.invitesPageSize;
		}   
		this.invitesPageIndex = event.pageIndex;
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
	}
	
	getLinks(lowValue, highValue) {
		this.receivedLinks = this.links.slice(lowValue, highValue);
	}

	getPaginatorLinks(event) {
		if(event.pageIndex === this.linksPageIndex + 1) {
			this.linksLowValue = this.linksLowValue + this.linksPageSize;
		   	this.linksHighValue =  this.linksHighValue + this.linksPageSize;
		} else if(event.pageIndex === this.linksPageIndex - 1) {
			this.linksLowValue= this.linksLowValue - this.linksPageSize;
			this.linksHighValue =  this.linksHighValue - this.linksPageSize;
		}   
		this.linksPageIndex = event.pageIndex;
		this.getLinks(this.linksLowValue, this.linksHighValue);
	}

	getSpaces(lowValue, highValue) {
		this.receivedSpaces = this.spaces.slice(lowValue, highValue);
	}

	getPaginatorSpaces(event) {
		if(event.pageIndex === this.spacesPageIndex + 1) {
			this.spacesLowValue = this.spacesLowValue + this.spacesPageSize;
		   	this.spacesHighValue =  this.spacesHighValue + this.spacesPageSize;
		} else if(event.pageIndex === this.spacesPageIndex - 1) {
			this.spacesLowValue= this.spacesLowValue - this.spacesPageSize;
			this.spacesHighValue =  this.spacesHighValue - this.spacesPageSize;
		}   
		this.spacesPageIndex = event.pageIndex;
		this.getSpaces(this.spacesLowValue, this.spacesHighValue);
	}

	onLinkCopyBtn(link: SpaceLink){
		let copyToClipboard = this.document.createElement('textarea');
    	copyToClipboard.style.position = 'fixed';
    	copyToClipboard.style.left = '0';
    	copyToClipboard.style.top = '0';
    	copyToClipboard.style.opacity = '0';
    	copyToClipboard.value = this.document.location.origin + this.linksURL + link.name;
		document.body.appendChild(copyToClipboard);
		copyToClipboard.focus();
		copyToClipboard.select();
		document.execCommand('copy');
		document.body.removeChild(copyToClipboard);
	}

	onInviteDelBtn(invite: SpaceInvitation){
		const index = this.invites.findIndex(x => x.id === invite.id);
		if (index > -1) {
			this.invites.splice(index, 1);
		}
		this.invites = [...this.invites];
		this.invitesLowValue = 0;
		this.invitesHighValue = 7;
		this.invitesPageIndex = 0;
		if (this.invitesPaginator) {
			this.invitesPaginator.firstPage();
		}
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
	}

	onLinkDelBtn(link: SpaceLink){
		const index = this.links.findIndex(x => x.id === link.id);
		if (index > -1) {
			this.links.splice(index, 1);
		}
		this.links = [...this.links];
		this.linksLowValue = 0;
		this.linksHighValue = 7;
		this.linksPageIndex = 0;
		if (this.linksPaginator) {
			this.linksPaginator.firstPage();
		}
		this.getLinks(this.linksLowValue, this.linksHighValue);
	}

	onSpaceDelBtn(space: SpaceInterface){
		const index = this.spaces.findIndex(x => x.key === space.key);
		if (index > -1) {
			this.spaces.splice(index, 1);
		}
		this.spaces = [...this.spaces];
		this.spacesLowValue = 0;
		this.spacesHighValue = 7;
		this.spacesPageIndex = 0;
		if (this.spacesPaginator) {
			this.spacesPaginator.firstPage();
		}
		// maybe it'd better to put getSpaces to "else" clause of paginator "if" from above for optimization puproses
		this.getSpaces(this.spacesLowValue, this.spacesHighValue);
	}

	private _initInviteForm() {
		this.inviteForm = this.fb.group({
			userId: ['', [
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

	onSpaceInviteAddUser(space: SpaceInterface) {
		this.isInviteFormHidden = false;
		this.inviteFormSpaceId = space.id;
		this.scrollToInviteForm.nativeElement.scrollIntoView();
	}

	onSpaceAddLink(space: SpaceInterface) {
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
	
		let newInvite: SpaceInvitation = {
			id: crypto.randomUUID(),
			spaceId: this.inviteFormSpaceId,
			userId: data.value.userId,
			role: data.value.role,
			creatorId: this.currentUser,
			createdAt: new Date()
		};

		// thinking that duplicate invites (with unique uuid) are not limited and may exist. Is it right?
		this.invites.push(newInvite);
		this.invites = [...this.invites];
		this.invitesLowValue = 0;
		this.invitesHighValue = 7;
		this.invitesPageIndex = 0;
		this.inviteForm.reset();
		this.isInviteFormHidden = true;
		if (this.invitesPaginator) {
			this.invitesPaginator.firstPage();
		}
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
	}

	onSubmitLink(data): void {
		if (this.linkForm.invalid) {
			return;
		}
	
		let newLink: SpaceLink = {
			id: crypto.randomUUID(),
			spaceId: this.linkFormSpaceId,
			creatorId: this.currentUser,
			name: data.value.name,
			createdAt: new Date(),
			expiredAt: data.value.expiredAt
		};

		// I assume that duplicate links may exist.
		this.links.push(newLink);
		this.links = [...this.links];
		this.linksLowValue = 0;
		this.linksHighValue = 7;
		this.linksPageIndex = 0;
		this.linkForm.reset();
		this.isLinkFormHidden = true;
		if (this.linksPaginator) {
			this.linksPaginator.firstPage();
		}
		this.getLinks(this.linksLowValue, this.linksHighValue);
	}

	getSpaceNameById(spaceId: string) {
		let space = this.spaces.find(s => s.id === spaceId);
		if (space) {
			return space.name;
		} else {
			return null;
		}
	}
}
