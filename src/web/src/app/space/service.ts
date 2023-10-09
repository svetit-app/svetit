import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {Space, SpaceListResponse} from './model';
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

	createNewSpace(name: string, key: string, requestsAllowed: boolean) {
		this.spaces.push({
			id: crypto.randomUUID(),
			name: name,
			key: key,
			requestsAllowed: requestsAllowed,
			createdAt: new Date()
		});
	}
}
