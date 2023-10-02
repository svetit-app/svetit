import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { NgFor } from '@angular/common';

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
		{space: 'Пространство №1', name: "ref123", date: "19.04.2024"},
		{space: 'Пространство №2', name: "ref456", date: "27.05.2025"},
		{space: 'Пространство №3', name: "ref789", date: "03.10.2023"},
		{space: 'Пространство №4', name: "ref123123", date: "11.11.2024"},
		{space: 'Пространство №5', name: "ref456456", date: "07.07.2025"},
		{space: 'Пространство №6', name: "ref789789", date: "23.01.2023"},
		{space: 'Пространство №7', name: "ref1231", date: "09.05.2024"},
		{space: 'Пространство №8', name: "ref4562", date: "17.09.2025"},
		{space: 'Пространство №9', name: "ref7893", date: "13.12.2023"},
	];

	spaces: Space[] = [
		{name: 'Пространство №1', key: 'key13219'},
		{name: 'Пространство №2', key: 'key41983'},
		{name: 'Пространство №3', key: 'key123asd'},
		{name: 'Пространство №4', key: 'key41324s'},
		{name: 'Пространство №5', key: 'key865211'},
		{name: 'Пространство №6', key: 'key4a2wwa'},
		{name: 'Пространство №7', key: 'keyasdaw9'},
		{name: 'Пространство №8', key: 'keyasd467'},
		{name: 'Пространство №9', key: 'key532sg5'},
	];

	receivedInvites: SpaceInvite[] = [];
	receivedRefs: SpaceRef[] = [];
	receivedSpaces: Space[] = [];

	constructor(
		private route: ActivatedRoute,
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
}
