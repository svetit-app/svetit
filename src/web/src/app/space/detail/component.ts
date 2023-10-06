import { Component, OnInit, ViewChild, Inject, Injectable } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';

import { SpaceInvitation } from '../model';
import { SpaceLink } from '../model';
import { SpaceUser } from '../model';
import { SpaceUserAddinitionalInfoFromAuth } from '../model';

@Component({
	selector: 'app-space-detail',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceDetailComponent implements OnInit {

	title: string = "Пространство №1";

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/links/";

	// question - is it needed to copy the same interface for invites listing as at space list page with the same templating logic?
	// question - what types of invites should appear in UI at space detail page?
	invites: SpaceInvitation[] = [
		// Меня пригласили
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-01")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-02")},
		// Я прошусь
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-03")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-04")},
		// Мы пригласили
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-05")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "petya", role: "guest", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-06")},
		// Хочет к нам
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-07")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-09")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-10")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-11")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-12")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-13")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-14")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-15")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-16")},
	];

	links: SpaceLink[] = [
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link1", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "vasya", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	];

	users: SpaceUser[] = [
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "petya", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "vasya", isOwner: true, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "kolya", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "olgaiv", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "ekapet", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "sergnik", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "ssemen", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "olegk", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "vasilisa", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "krisn", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "ekaserg", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "niknik", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "olegsid", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "vassem", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "elenaleks", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
	];

	usersAddInfo: SpaceUserAddinitionalInfoFromAuth[] = [
		{name: "Петр Петрович", userId: "petya", email: "petya@example.com"},
		{name: "Василий Иванович", userId: "vasya", email: "vasya@example.com"},
		{name: "Николай Александрович", userId: "kolya", email: "kolya@example.com"},
		{name: "Ольга Ивановна", userId: "olgaiv", email: "olgaiv@example.com"},
		{name: "Екатерина Петровна", userId: "ekapet", email: "ekapet@example.com"},
		{name: "Сергей Николаевич", userId: "sergnik", email: "sergnik@example.com"},
		{name: "Семён Семёныч", userId: "ssemen", email: "ssemen@example.com"},
		{name: "Олег Китаич", userId: "olegk", email: "olegk@example.com"},
		{name: "Василиса Александровна", userId: "vasilisa",email: "vasilisa@example.com"},
		{name: "Кристина Николаевна", userId: "krisn", email: "krisn@example.com"},
		{name: "Екатерина Сергеевна", userId: "ekaserg", email: "ekaserg@example.com"},
		{name: "Николай Николаевич", userId: "niknik", email: "niknik@example.com"},
		{name: "Олег Сидорович", userId: "olegsid", email: "olegsid@example.com"},
		{name: "Василий Семёнович", userId: "vassem", email: "vassem@example.com"},
		{name: "Елена Александровна", userId: "elenaleks", email: "elenaleks@example.com"},
	];

	receivedInvites: SpaceInvitation[] = [];
	receivedLinks: SpaceLink[] = [];
	receivedUsers: SpaceUser[] = [];

	invitesPageIndex: number = 0;
    invitesPageSize: number = 7;
    invitesLowValue: number = 0;
    invitesHighValue: number = 7;  

	linksPageIndex: number = 0;
    linksPageSize: number = 7;
    linksLowValue: number = 0;
    linksHighValue: number = 7;

	usersPageIndex: number = 0;
    usersPageSize: number = 10;
    usersLowValue: number = 0;
    usersHighValue: number = 10;

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any
	) {
	}

	ngOnInit() {
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
		this.getLinks(this.linksLowValue, this.linksHighValue);
		this.getUsers(this.usersLowValue, this.usersHighValue);
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

	getUsers(lowValue, highValue) {
		this.receivedUsers = this.users.slice(lowValue, highValue);
	}

	getPaginatorUsers(event) {
		if(event.pageIndex === this.usersPageIndex + 1) {
			this.usersLowValue = this.usersLowValue + this.usersPageSize;
		   	this.usersHighValue =  this.usersHighValue + this.usersPageSize;
		} else if(event.pageIndex === this.usersPageIndex - 1) {
			this.usersLowValue= this.usersLowValue - this.usersPageSize;
			this.usersHighValue =  this.usersHighValue - this.usersPageSize;
		}   
		this.usersPageIndex = event.pageIndex;
		this.getUsers(this.usersLowValue, this.usersHighValue);
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

	onUserDelBtn(user: SpaceUser){
		const index = this.users.findIndex(x => x.userId === user.userId);
		if (index > -1) {
			this.users.splice(index, 1);
		}
		this.users = [...this.users];
		this.usersLowValue = 0;
		this.usersHighValue = 7;
		this.usersPageIndex = 0;
		if (this.usersPaginator) {
			this.usersPaginator.firstPage();
		}
		this.getUsers(this.usersLowValue, this.usersHighValue);
	}

	getUserNameById(userId: string) {
		let userAddInfo = this.usersAddInfo.find(u => u.userId === userId);
		if (userAddInfo) {
			return userAddInfo.name;
		} else {
			return null;
		}
	}

	getUserEmailById(userId: string) {
		let userAddInfo = this.usersAddInfo.find(u => u.userId === userId);
		if (userAddInfo) {
			return userAddInfo.email;
		} else {
			return null;
		}
	}
}
