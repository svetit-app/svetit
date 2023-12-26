import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {of} from 'rxjs';
import {switchMap, delay} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User, UserFields} from './model';
import {SpaceService} from '../space/service';
import { RequestWatcherService } from '../request-watcher/service';
import {PaginatorApi} from '../user';

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

	constructor(
		private spaceSrv: SpaceService,
		private http: HttpClient,
		private requestWatcher: RequestWatcherService,
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

	getById(userId: string): Observable<User> {
		return this.http.get<User>(this._apiUrl + "/" + userId).pipe(
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

	getList(limit: number, page: number, login: string = ''): Observable<User[]> {
		return this.http.get<User[]>(this._apiUrl + "/list?start=" + limit*page + "&limit=" + limit + "&search=" + login).pipe(
			src => this.requestWatcher.WatchFor(src),
		);
	}
}
