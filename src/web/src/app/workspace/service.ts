import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {Workspace, WorkspaceListResponse} from './model';
import { NavigationExtras, Router } from '@angular/router';

@Injectable()
export class WorkspaceService {
	private _isInitialized: ReplaySubject<boolean> = new ReplaySubject();
	private _current: Workspace = null;
	private _items: Workspace[] = [];

	private _apiUrl = '/api/workspace/';

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

	Fetch(): Observable<boolean> {
		return this.http.get<WorkspaceListResponse>(this._apiUrl + 'list').pipe(
			switchMap(res => {
				this._items = res.items;

				if (!this._items.length) {
					let extras: NavigationExtras = {};
					if (this.router.url !== '' && this.router.url !== '/')
						extras['queryParams'] = {'redirectPath': this.router.url};
					this.router.navigate(['/hello'], extras);

					return of(false);
				}

				this._isInitialized.next(true);
				return of(true);
			})
		);
	}

	isInitialized(): Observable<boolean> {
		return this._isInitialized.asObservable();
	}
}
