import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

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
		{name: "ref21322", date: "19.12.23"},
		{name: "ref98123", date: "10.07.24"},
		{name: "ref74332", date: "04.03.25"},
		{name: "ref092dd", date: "11.11.23"},
		{name: "ref812as", date: "15.08.24"},
		{name: "ref4993s", date: "24.01.25"},
		{name: "ref9i65u", date: "09.10.24"},
		{name: "refa92131", date: "11.02.25"},
		{name: "ref0901waa", date: "14.05.24"},
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

	constructor(
		private route: ActivatedRoute,
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
}
