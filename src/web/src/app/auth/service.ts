import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, concatMap, switchMap, delay, map} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User, UserFields} from './model';
import {SpaceService} from '../space/service';
import {RequestWatcherService} from '../request-watcher/service';
import {PaginatorApi} from '../user';
import { AuthService as ApiAuthService } from '../api';
import { UserInfo as ApiUserInfo} from '../api';


@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject(1);
	private _user: ApiUserInfo = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	get user() {
		return this._user;
	}

	get space() {
		return this.spaceSrv.current;
	}

	constructor(
		private spaceSrv: SpaceService,
		private http: HttpClient,
		private requestWatcher: RequestWatcherService,
		private apiAuthService: ApiAuthService,
	) {
	}

	GoToLogin(): void {
		window.location.href = window.location.origin + "/api/auth/login";
	}

	GoToLogout(): void {
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
		// todo - разобраться, что делать с запросом айдишника сессии в параметрах, в доке он есть, и теперь фронтовый метод его тоже хочет
		return this.apiAuthService.handlerUserInfoGet("session").pipe(
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

	getById(userId: string): Observable<ApiUserInfo> {
		return this.apiAuthService.handlerUserByidGet("session", userId).pipe(
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

	getList(limit: number, page: number, login: string = ''): Observable<ApiUserInfo[]> {
		return this.apiAuthService.handlerUserListGet("session", limit*page, limit, login).pipe(
			src => this.requestWatcher.WatchFor(src),
			map(res => {
				return res["list"];
			}),
		)
	}
}
