import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {Space, SpaceListResponse} from './model';
import { NavigationExtras, Router } from '@angular/router';

@Injectable()
export class SpaceService {
	private _isChecked = false;
	private _isInitialized: ReplaySubject<boolean> = new ReplaySubject();
	private _current: Space = null;
	private _items: Space[] = [];

	private _apiUrl = '/api/space/';

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
}
