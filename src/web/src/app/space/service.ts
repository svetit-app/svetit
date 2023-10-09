import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceInvitation, SpaceLink, SpaceListResponse } from './model';
import { NavigationExtras, Router } from '@angular/router';
import { PaginatorApi } from '../user';

@Injectable()
export class SpaceService {
	private _isChecked = false;
	private _isInitialized: ReplaySubject<boolean> = new ReplaySubject();
	private _current: Space = null;
	private _items: Space[] = [];

	private _apiUrl = '/api/space/';

	spaces: Space[] = [
		{id: "11111111-1111-1111-1111-111111111111", name: "Пространство №1", key: "key1", requestsAllowed: true, createdAt: new Date("2023-10-01")},
		{id: "22222222-2222-2222-2222-222222222222", name: "Пространство №2", key: "key2", requestsAllowed: true, createdAt: new Date("2023-10-02")},
		{id: "33333333-3333-3333-3333-333333333333", name: "Пространство №3", key: "key3", requestsAllowed: true, createdAt: new Date("2023-10-03")},
		{id: "44444444-4444-4444-4444-444444444444", name: "Пространство №4", key: "key4", requestsAllowed: true, createdAt: new Date("2023-10-04")},
		{id: "55555555-5555-5555-5555-555555555555", name: "Пространство №5", key: "key5", requestsAllowed: true, createdAt: new Date("2023-10-05")},
		{id: "66666666-6666-6666-6666-666666666666", name: "Пространство №6", key: "key6", requestsAllowed: true, createdAt: new Date("2023-10-06")},
		{id: "77777777-7777-7777-7777-777777777777", name: "Пространство №7", key: "key7", requestsAllowed: true, createdAt: new Date("2023-10-07")},
		{id: "88888888-8888-8888-8888-888888888888", name: "Пространство №8", key: "key8", requestsAllowed: true, createdAt: new Date("2023-10-08")},
		{id: "99999999-9999-9999-9999-999999999999", name: "Пространство №9", key: "key9", requestsAllowed: true, createdAt: new Date("2023-10-09")},
		{id: "10000000-1000-1000-1000-100000000000", name: "Пространство №10", key: "key10", requestsAllowed: true, createdAt: new Date("2023-10-09")},
		{id: "11000000-1100-1100-1100-110000000000", name: "Пространство №11", key: "key11", requestsAllowed: true, createdAt: new Date("2023-10-09")},
	];

	invites: SpaceInvitation[] = [
		// Меня пригласили
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-01")},
		{id: Math.random(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "vasya", role: "user", creatorId: "anotherAdmin", createdAt: new Date("2023-10-02")},
		// Я прошусь
		{id: Math.random(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-03")},
		{id: Math.random(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "vasya", role: "", creatorId: "vasya", createdAt: new Date("2023-10-04")},
		// Мы пригласили
		{id: Math.random(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "kolya", role: "user", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-05")},
		{id: Math.random(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "petya", role: "guest", creatorId: "anotherColleagueAdmin2", createdAt: new Date("2023-10-06")},
		// Хочет к нам
		{id: Math.random(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-07")},
		{id: Math.random(), spaceId: '88888888-8888-8888-8888-888888888888', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-08")},
		{id: Math.random(), spaceId: '99999999-9999-9999-9999-999999999999', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-09")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-10")},
		{id: Math.random(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-11")},
		{id: Math.random(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-12")},
		{id: Math.random(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-13")},
		{id: Math.random(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-14")},
		{id: Math.random(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "kolya", role: "user", creatorId: "kolya", createdAt: new Date("2023-10-15")},
		{id: Math.random(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "lena", role: "guest", creatorId: "lena", createdAt: new Date("2023-10-16")},
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

	get current() {
		return this._current;
	}

	get items() {
		return this._items;
	}

	constructor(
		private http: HttpClient,
		private router: Router,
	) {
	}

	Check(): Observable<boolean> {
		if (this._isChecked)
			return this.isInitialized();
		return this.Fetch();
	}

	Fetch(): Observable<boolean> {
		this._isChecked = true;
		return this.http.get<SpaceListResponse>(this._apiUrl + 'list').pipe(
			switchMap(res => {
				this._items = res.items;
				this._isInitialized.next(true);
				return of(true);
			})
		);
	}

	isInitialized(): Observable<boolean> {
		return this._isInitialized.asObservable();
	}

	getSpaceList(limit: number, page: number): Observable<PaginatorApi<Space>> {
		const res: PaginatorApi<Space> = {
			count: this.spaces.length,
			results: this.spaces.slice(limit * page, limit * page + limit),
		};
		return of(res);
	}

	getSpaceListAll() {
		const res: PaginatorApi<Space> = {
			count: this.spaces.length,
			results: this.spaces,
		};
		return of(res);
	}

	getSpaceNameById(spaceId: string) {
		let space = this.spaces.find(s => s.id === spaceId);
		return of(space?.name);
	}

	getInvitesList(limit: number, page: number): Observable<PaginatorApi<SpaceInvitation>> {
		const res: PaginatorApi<SpaceInvitation> = {
			count: this.invites.length,
			results: this.invites.slice(limit * page, limit * page + limit),
		};
		return of(res);
	}

	getLinksList(limit: number, page: number): Observable<PaginatorApi<SpaceLink>> {
		const res: PaginatorApi<SpaceLink> = {
			count: this.links.length,
			results: this.links.slice(limit * page, limit * page + limit),
		};
		return of(res);
	}

	isSpaceExists(key: string) {
		return this.spaces.some(s => s.key === key);
	}

	createNewSpace(name: string, key: string, requestsAllowed: boolean) {
		this.spaces.push({
			id: crypto.randomUUID(),
			name: name,
			key: key,
			requestsAllowed: requestsAllowed,
			createdAt: new Date()
		});
	}

	createNewInvite(spaceId: string, userId: string, role: string, creatorId: string) {
		this.invites.push({
			id: Math.random(),
			spaceId: spaceId,
			userId: userId,
			role: role,
			creatorId: creatorId,
			createdAt: new Date(),
		});
	}

	createNewLink(spaceId: string, creatorId: string, name: string, expiredAt: Date) {
		this.links.push({
			id: crypto.randomUUID(),
			spaceId: spaceId,
			creatorId: creatorId,
			name: name,
			createdAt: new Date(),
			expiredAt: expiredAt
		});
	}

	delInviteById(inviteId: number) {
		const index = this.invites.findIndex(x => x.id === inviteId);
		if (index > -1) {
			this.invites.splice(index, 1);
		}
	}

	delLinkById(linkId: string) {
		const index = this.links.findIndex(x => x.id === linkId);
		if (index > -1) {
			this.links.splice(index, 1);
		}
	}

	delSpaceById(spaceId: string) {
		const index = this.spaces.findIndex(x => x.id === spaceId);
		if (index > -1) {
			this.spaces.splice(index, 1);
		}
	}
}
