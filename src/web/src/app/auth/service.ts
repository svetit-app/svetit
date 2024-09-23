import { Injectable, inject } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, concatMap, switchMap, delay, map} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User, UserFields} from './model';
import {SpaceService} from '../space/service';
import {RequestWatcherService} from '../request-watcher/service';
import {PaginatorApi} from '../user';
import { AuthService as ApiAuthService, UserInfo } from '../api';
import { SpaceVisitHolder } from '../space/visit-holder';


@Injectable()
export class AuthService {
	private spaceSrv = inject(SpaceService);
	private spaceVisitHolder = inject(SpaceVisitHolder);
	private http = inject(HttpClient);
	private requestWatcher = inject(RequestWatcherService);
	private api = inject(ApiAuthService);

	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject(1);
	private _user: UserInfo = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	get user() {
		return this._user;
	}

	get space() {
		return this.spaceSrv.current;
	}

	GoToLogin(hideReferrer: boolean = false): void {
		const loginUrl = window.location.origin + "/api/auth/login";

		if (hideReferrer) {
			window.open(loginUrl, '_self', 'noreferrer');
			return;
		}

		window.location.href = loginUrl;
	}

	GoToLogout(): void {
		this.spaceVisitHolder.clear();
		window.location.href = window.location.origin + "/api/auth/logout";
	}

	Check(): Observable<boolean> {
		if (this._isChecked)
			return this.isAuthorized();
		return this.FetchUser();
	}

	isAuthorized(): Observable<boolean> {
		return this._isAuthorized.asObservable();
	}

	FetchUser(): Observable<boolean> {
		this._isChecked = true;
		return this.api.userInfoGet().pipe(
			switchMap(res => {
				this._user = res;
				this._isAuthorized.next(true);
				return of(true);
			})
		);
	}

	isAdmin(): boolean {
		return this.checkPermission('change_logentry');
	}

	canEditScheme(): boolean {
		return this.checkPermission('edit_scheme');
	}

	canRestart(): boolean {
		return this.checkPermission('restart');
	}

	isExtraList(): boolean {
		return this.checkPermission('extra_list');
	}

	canChangeMode(): boolean {
		return this.checkPermission('change_dig_mode');
	}

	canChangeParam(): boolean {
		return this.checkPermission('change_dig_param_value');
	}

	canChangeValue(): boolean {
		return this.checkPermission('change_device_item_value');
	}

	checkPermission(item: string): boolean {
		return this._permissions?.indexOf(item) > -1;
	}

	getById(userId: string): Observable<UserInfo> {
		return this.api.userByidGet(userId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	fillFields<T extends UserFields & {userId: string}>(items: T[]) {
		const ids = new Set<string>();
		items.forEach(user => ids.add(user.userId));
		for (const id of ids) {
			this.getById(id).subscribe(user => {
				for (const item of items) {
					if (item.userId !== user.id)
						continue;
					item.userDisplayName = user.displayName;
					item.userLogin = user.login;
					item.userEmail = user.email;
				}
			});
		}
	}

	getList(limit: number, page: number, login: string = ''): Observable<UserInfo[]> {
		return this.api.userListGet(limit*page, limit, login).pipe(
			src => this.requestWatcher.WatchFor(src),
			map(res => res.list),
		)
	}
}
