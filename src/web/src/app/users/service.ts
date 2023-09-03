import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User,UserInfo} from './model';
import { NavigationExtras, Router } from '@angular/router';

@Injectable()
export class UsersService {
	private _isInitialized: ReplaySubject<boolean> = new ReplaySubject();
	private _user: User;
	private _info: UserInfo;

	private _usersUrl = '/api/users/';

	get user() {
		return this._user;
	}

	constructor(
		private http: HttpClient,
		private router: Router,
	) {
	}

	SetUser(user: User) {
		this._user = user;

		this.http.get<UserInfo>(this._usersUrl + 'user/').pipe(
			catchError(err => {
				console.error("Get user info error:", err);
				// TODO: goto error page
				return throwError(err);
			})
		).subscribe(info => this.setUserInfo(info as UserInfo));
	}

	private setUserInfo(info: UserInfo) {
		console.log("Hello users info", info, "from:", this.router.url);
		if (!info?.unions?.length) {
			let extras: NavigationExtras = {};
			if (this.router.url !== '' && this.router.url !== '/')
				extras['queryParams'] = {'redirectPath': this.router.url};
    		this.router.navigate(['/hello'], extras);
			return;
		}

		this._info = info as UserInfo;
		this._isInitialized.next(true);
	}

	isInitialized(): Observable<boolean> {
		return this._isInitialized.asObservable();
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
		return this._info?.permissions?.indexOf(item) > -1;
	}
}
