import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

interface Invite {
	user: string;
	roles: string[];
}

interface Link {
	name: string;
	date: string;	
}

interface User {
	name: string;
	login: string;
	roles: string[];
}

@Component({
	selector: 'app-temp',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceDetailComponent implements OnInit {

	invites: Invite[] = [
		{user: "petya", roles: ["admin", "user", "guest"]},
		{user: "vasya", roles: ["admin", "user", "guest"]},
		{user: "kolya", roles: ["admin", "user", "guest"]},
	];

	links: Link[] = [
		{name: "ref21322", date: "19.12.23"},
		{name: "ref98123", date: "10.07.24"},
		{name: "ref74332", date: "04.03.25"},
	];

	users: User[] = [
		{name: "Петр Петрович", login: "petya", roles: ["admin", "user", "guest"]},
		{name: "Василий Иванович", login: "vasya", roles: ["admin", "user", "guest"]},
		{name: "Николай Александрович", login: "kolya", roles: ["admin", "user", "guest"]},
	];

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}
