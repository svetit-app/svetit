import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {NavigationExtras, Router} from '@angular/router';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User} from './model';
import {SpaceService} from '../space/service';

@Injectable()
export class UserService {
	private _info: User = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	get info() {
		return this._info;
	}

	get space() {
		return this.spaceSrv.current;
	}

	get spaceList() {
		return this.spaceSrv.items;
	}

	constructor(
		private spaceSrv: SpaceService,
		private http: HttpClient,
		private router: Router,
	) {
	}

	FetchInfo(): Observable<boolean> {
		return this.http.get(this._apiUrl + 'info').pipe(
			switchMap(res => {
				this._info = res as User;
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
}
