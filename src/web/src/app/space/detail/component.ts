import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

interface Invite {
	user: string;
	role: string;
}

interface Link {
	name: string;
	date: string;	
}

interface User {
	name: string;
	login: string;
	role: string;
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
	];

	links: Link[] = [
		{name: "ref21322", date: "19.12.23"},
		{name: "ref98123", date: "10.07.24"},
		{name: "ref74332", date: "04.03.25"},
	];

	users: User[] = [
		{name: "Петр Петрович", login: "petya", role: "admin"},
		{name: "Василий Иванович", login: "vasya", role: "user"},
		{name: "Николай Александрович", login: "kolya", role: "guest"},
	];

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}
