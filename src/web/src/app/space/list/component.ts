import { Component, OnInit, Inject, Injectable, ViewChild } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { NgFor, DOCUMENT } from '@angular/common';
import { MatPaginator } from '@angular/material/paginator';

interface SpaceInvite {
	spaceId: string;
	userId: string;
	role: string;
	creatorId: string;
}

interface SpaceRef {
	space: string;
	name: string;
	date: string;
}

interface Space {
	name: string;
	key: string;
}

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {

	invitesPageIndex: number = 0;
    invitesPageSize: number = 7;
    invitesLowValue: number = 0;
    invitesHighValue: number = 7;  

	refsPageIndex: number = 0;
    refsPageSize: number = 7;
    refsLowValue: number = 0;
    refsHighValue: number = 7;

	spacesPageIndex: number = 0;
    spacesPageSize: number = 7;
    spacesLowValue: number = 0;
    spacesHighValue: number = 7;

	// userId текущего залогиненного юзера
	currentUser: string = "vasya";

	// относительный адрес для ссылок-приглашений
	refsURL: string = "/refs/";

	spaceinvites: SpaceInvite[] = [
		// Меня пригласили
		{spaceId: 'Пространство №1', userId: "vasya", role: "user", creatorId: "anotherAdmin"},
		{spaceId: 'Пространство №2', userId: "vasya", role: "user", creatorId: "anotherAdmin"},
		// Я прошусь
		{spaceId: 'Пространство №3', userId: "vasya", role: "", creatorId: "vasya"},
		{spaceId: 'Пространство №4', userId: "vasya", role: "", creatorId: "vasya"},
		// Мы пригласили
		{spaceId: 'Пространство №5', userId: "kolya", role: "user", creatorId: "anotherColleagueAdmin2"},
		{spaceId: 'Пространство №6', userId: "petya", role: "guest", creatorId: "anotherColleagueAdmin2"},
		// Хочет к нам
		{spaceId: 'Пространство №7', userId: "kolya", role: "user", creatorId: "kolya"},
		{spaceId: 'Пространство №8', userId: "lena", role: "guest", creatorId: "lena"},
		{spaceId: 'Пространство №9', userId: "kolya", role: "user", creatorId: "kolya"},
		{spaceId: 'Пространство №10', userId: "lena", role: "guest", creatorId: "lena"},
		{spaceId: 'Пространство №11', userId: "kolya", role: "user", creatorId: "kolya"},
		{spaceId: 'Пространство №12', userId: "lena", role: "guest", creatorId: "lena"},
		{spaceId: 'Пространство №13', userId: "kolya", role: "user", creatorId: "kolya"},
		{spaceId: 'Пространство №14', userId: "lena", role: "guest", creatorId: "lena"},
		{spaceId: 'Пространство №15', userId: "kolya", role: "user", creatorId: "kolya"},
		{spaceId: 'Пространство №16', userId: "lena", role: "guest", creatorId: "lena"},
	];

	spacerefs: SpaceRef[] = [
		{space: 'Пространство №1', name: "ref1", date: "19.04.2024"},
		{space: 'Пространство №2', name: "ref2", date: "27.05.2025"},
		{space: 'Пространство №3', name: "ref3", date: "03.10.2023"},
		{space: 'Пространство №4', name: "ref4", date: "11.11.2024"},
		{space: 'Пространство №5', name: "ref5", date: "07.07.2025"},
		{space: 'Пространство №6', name: "ref6", date: "23.01.2023"},
		{space: 'Пространство №7', name: "ref7", date: "09.05.2024"},
		{space: 'Пространство №8', name: "ref8", date: "17.09.2025"},
		{space: 'Пространство №9', name: "ref9", date: "13.12.2023"},
		{space: 'Пространство №10', name: "ref10", date: "19.04.2024"},
		{space: 'Пространство №11', name: "ref11", date: "19.04.2024"},
		{space: 'Пространство №12', name: "ref12", date: "27.05.2025"},
		{space: 'Пространство №13', name: "ref13", date: "03.10.2023"},
		{space: 'Пространство №14', name: "ref14", date: "11.11.2024"},
		{space: 'Пространство №15', name: "ref15", date: "07.07.2025"},
		{space: 'Пространство №16', name: "ref16", date: "23.01.2023"},
		{space: 'Пространство №17', name: "ref17", date: "09.05.2024"},
		{space: 'Пространство №18', name: "ref18", date: "17.09.2025"},
		{space: 'Пространство №19', name: "ref19", date: "13.12.2023"},
	];

	spaces: Space[] = [
		{name: 'Пространство №1', key: 'key1'},
		{name: 'Пространство №2', key: 'key2'},
		{name: 'Пространство №3', key: 'key3'},
		{name: 'Пространство №4', key: 'key4'},
		{name: 'Пространство №5', key: 'key5'},
		{name: 'Пространство №6', key: 'key6'},
		{name: 'Пространство №7', key: 'key7'},
		{name: 'Пространство №8', key: 'key8'},
		{name: 'Пространство №9', key: 'key9'},
	];

	receivedInvites: SpaceInvite[] = [];
	receivedRefs: SpaceRef[] = [];
	receivedSpaces: Space[] = [];

	@ViewChild('invitesPaginator') invitesPaginator: MatPaginator;
	@ViewChild('refsPaginator') refsPaginator: MatPaginator;
	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;

	constructor(
		private route: ActivatedRoute,
		@Inject(DOCUMENT) private document: any
	) {
	}

	ngOnInit() {
		this.getInvites(this.invitesLowValue, this.invitesHighValue);
		this.getRefs(this.refsLowValue, this.refsHighValue);
		this.getSpaces(this.spacesLowValue, this.spacesHighValue);
	}

	getInvites(lowValue, highValue) {
		this.receivedInvites = this.spaceinvites.slice(lowValue, highValue);
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
		this.receivedRefs = this.spacerefs.slice(lowValue, highValue);
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

	onRefCopyBtn(ref: SpaceRef){
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

	onInviteDelBtn(invite: SpaceInvite){
		const index = this.spaceinvites.findIndex(x => x.spaceId === invite.spaceId);
		if (index > -1) {
			this.spaceinvites.splice(index, 1);
		}
		this.spaceinvites = [...this.spaceinvites];
		this.invitesLowValue = 0;
		this.invitesHighValue = 7;
		this.invitesPageIndex = 0;
		if (this.invitesPaginator) {
			this.invitesPaginator.firstPage();
		}
		this.getInvites(this.spacesLowValue, this.spacesHighValue);
	}

	onRefDelBtn(ref: SpaceRef){
		const index = this.spacerefs.findIndex(x => x.name === ref.name);
		if (index > -1) {
			this.spacerefs.splice(index, 1);
		}
		this.spacerefs = [...this.spacerefs];
		this.refsLowValue = 0;
		this.refsHighValue = 7;
		this.refsPageIndex = 0;
		if (this.refsPaginator) {
			this.refsPaginator.firstPage();
		}
		this.getRefs(this.refsLowValue, this.refsHighValue);
	}

	onSpaceDelBtn(space: Space){
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
		this.getSpaces(this.spacesLowValue, this.spacesHighValue);
	}
}
