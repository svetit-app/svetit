import { Component, OnInit, ViewChild, Inject, Injectable } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { MatPaginator } from '@angular/material/paginator';
import { DOCUMENT } from '@angular/common';

interface Invite {
	user: string;
	role: string;
}

interface Ref {
	name: string;
	date: string;	
}

interface User {
	name: string;
	login: string;
	role: string;
	email: string;
}

@Component({
	selector: 'app-space-detail',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceDetailComponent implements OnInit {

	title: string = "Пространство №1";

	// относительный адрес для ссылок-приглашений
	refsURL: string = "/refs/";

	invites: Invite[] = [
		{user: "petya", role: "admin"},
		{user: "vasya", role: "user"},
		{user: "kolya", role: "guest"},
		{user: "jenya", role: "admin"},
		{user: "elena", role: "user"},
		{user: "olya", role: "guest"},
		{user: "vanya", role: "admin"},
		{user: "katya", role: "user"},
		{user: "serg", role: "guest"},
	];

	refs: Ref[] = [
		{name: "ref1", date: "19.12.23"},
		{name: "ref2", date: "10.07.24"},
		{name: "ref3", date: "04.03.25"},
		{name: "ref4", date: "11.11.23"},
		{name: "ref5", date: "15.08.24"},
		{name: "ref6", date: "24.01.25"},
		{name: "ref7", date: "09.10.24"},
		{name: "ref8", date: "11.02.25"},
		{name: "ref9", date: "14.05.24"},
		{name: "ref10", date: "19.12.23"},
		{name: "ref11", date: "19.12.23"},
		{name: "ref12", date: "10.07.24"},
		{name: "ref13", date: "04.03.25"},
		{name: "ref14", date: "11.11.23"},
		{name: "ref15", date: "15.08.24"},
		{name: "ref16", date: "24.01.25"},
		{name: "ref17", date: "09.10.24"},
		{name: "ref18", date: "11.02.25"},
		{name: "ref19", date: "14.05.24"},
		{name: "ref20", date: "19.12.23"},
	];

	users: User[] = [
		{name: "Петр Петрович", login: "petya", role: "admin", email: "petya@example.com"},
		{name: "Василий Иванович", login: "vasya", role: "user", email: "vasya@example.com"},
		{name: "Николай Александрович", login: "kolya", role: "guest", email: "kolya@example.com"},
		{name: "Ольга Ивановна", login: "olgaiv", role: "admin", email: "olgaiv@example.com"},
		{name: "Екатерина Петровна", login: "ekapet", role: "user", email: "ekapet@example.com"},
		{name: "Сергей Николаевич", login: "sergnik", role: "guest", email: "sergnik@example.com"},
		{name: "Семён Семёныч", login: "ssemen", role: "admin", email: "ssemen@example.com"},
		{name: "Олег Китаич", login: "olegk", role: "user", email: "olegk@example.com"},
		{name: "Василиса Александровна", login: "vasilisa", role: "guest", email: "vasilisa@example.com"},
		{name: "Кристина Николаевна", login: "krisn", role: "admin", email: "krisn@example.com"},
		{name: "Екатерина Сергеевна", login: "ekaserg", role: "user", email: "ekaserg@example.com"},
		{name: "Николай Николаевич", login: "niknik", role: "guest", email: "niknik@example.com"},
		{name: "Олег Сидорович", login: "olegsid", role: "admin", email: "olegsid@example.com"},
		{name: "Василий Семёнович", login: "vassem", role: "user", email: "vassem@example.com"},
		{name: "Елена Александровна", login: "elenaleks", role: "guest", email: "elenaleks@example.com"},
	];

	receivedInvites: Invite[] = [];
	receivedRefs: Ref[] = [];
	receivedUsers: User[] = [];

	invitesPageIndex: number = 0;
    invitesPageSize: number = 7;
    invitesLowValue: number = 0;
    invitesHighValue: number = 7;  

	refsPageIndex: number = 0;
    refsPageSize: number = 7;
    refsLowValue: number = 0;
    refsHighValue: number = 7;

	usersPageIndex: number = 0;
    usersPageSize: number = 7;
    usersLowValue: number = 0;
    usersHighValue: number = 7;

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('refsPaginator') refsPaginator: MatPaginator;
	@ViewChild('usersPaginator') usersPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any
	) {
	}

	ngOnInit() {
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
		this.getRefs(this.refsLowValue, this.refsHighValue);
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

	getRefs(lowValue, highValue) {
		this.receivedRefs = this.refs.slice(lowValue, highValue);
	}

	getPaginatorRefs(event) {
		if(event.pageIndex === this.refsPageIndex + 1) {
			this.refsLowValue = this.refsLowValue + this.refsPageSize;
		   	this.refsHighValue =  this.refsHighValue + this.refsPageSize;
		} else if(event.pageIndex === this.refsPageIndex - 1) {
			this.refsLowValue= this.refsLowValue - this.refsPageSize;
			this.refsHighValue =  this.refsHighValue - this.refsPageSize;
		}   
		this.refsPageIndex = event.pageIndex;
		this.getRefs(this.refsLowValue, this.refsHighValue);
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

	onRefCopyBtn(ref: Ref){
		let selBox = this.document.createElement('textarea');
    	selBox.style.position = 'fixed';
    	selBox.style.left = '0';
    	selBox.style.top = '0';
    	selBox.style.opacity = '0';
    	selBox.value = this.document.location.origin + this.refsURL + ref.name;
		document.body.appendChild(selBox);
		selBox.focus();
		selBox.select();
		document.execCommand('copy');
		document.body.removeChild(selBox);
	}

	onInviteDelBtn(invite: Invite){
		const index = this.invites.findIndex(x => x.user === invite.user);
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

	onRefDelBtn(ref: Ref){
		const index = this.refs.findIndex(x => x.name === ref.name);
		if (index > -1) {
			this.refs.splice(index, 1);
		}
		this.refs = [...this.refs];
		this.refsLowValue = 0;
		this.refsHighValue = 7;
		this.refsPageIndex = 0;
		if (this.refsPaginator) {
			this.refsPaginator.firstPage();
		}
		this.getRefs(this.refsLowValue, this.refsHighValue);
	}

	onUserDelBtn(user: User){
		const index = this.users.findIndex(x => x.login === user.login);
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
}
