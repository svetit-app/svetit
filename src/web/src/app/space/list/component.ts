import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';
import {NgFor} from '@angular/common';

interface SpaceInvite {
	name: string;
}

interface SpaceInviteMine {
	name: string;
	user: string;
	roles: string[];
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
	selector: 'app-temp',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {

	spaces: SpaceInvite[] = [
		{name: 'Пространство №1'},
		{name: 'Пространство №2'},
		{name: 'Пространство №3'},
	];

	spacesmine: SpaceInviteMine[] = [
		{name: 'Пространство №1', user: "vasya", roles: ["admin", "user", "guest"]},
		{name: 'Пространство №2', user: "petya", roles: ["admin", "user", "guest"]},
		{name: 'Пространство №3', user: "kolya", roles: ["admin", "user", "guest"]},
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
}
