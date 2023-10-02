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

interface SpacesList {
	name: string;
	key: string;
}

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {

	pageIndex: number = 0;
    pageSize: number = 7;
    lowValue: number = 0;
    highValue: number = 7;  

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
	];

	spaceslist: SpacesList[] = [
		{name: 'Пространство №1', key: 'key13219'},
		{name: 'Пространство №2', key: 'key41983'},
		{name: 'Пространство №3', key: 'key92333'},
	];

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}

	getPaginatorDataInvites(event){
		console.log(event);
		if(event.pageIndex === this.pageIndex + 1){
		   this.lowValue = this.lowValue + this.pageSize;
		   this.highValue =  this.highValue + this.pageSize;
		  }
	   else if(event.pageIndex === this.pageIndex - 1){
		  this.lowValue = this.lowValue - this.pageSize;
		  this.highValue =  this.highValue - this.pageSize;
		 }   
		  this.pageIndex = event.pageIndex;
	}
}
