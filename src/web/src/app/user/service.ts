import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {NavigationExtras, Router} from '@angular/router';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap, delay} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User, UserFields} from './model';
import {SpaceService} from '../space/service';

@Injectable()
export class UserService {
	private _info: User = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	private _mockDataForSpaceDetailPage: User[] = [
		{id: "1", displayName: "Петр Петрович", login: "petya", email: "petya@example.com", firstname: "", lastname: ""},
		{id: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", displayName: "Василий Иванович", login: "vasya", email: "vasya@example.com", firstname: "", lastname: ""},
		{id: "3", displayName: "Николай Александрович", login: "kolya", email: "kolya@example.com", firstname: "", lastname: ""},
		{id: "4", displayName: "Ольга Ивановна", login: "olgaiv", email: "olgaiv@example.com", firstname: "", lastname: ""},
		{id: "5", displayName: "Екатерина Петровна", login: "ekapet", email: "ekapet@example.com", firstname: "", lastname: ""},
		{id: "6", displayName: "Сергей Николаевич", login: "sergnik", email: "sergnik@example.com", firstname: "", lastname: ""},
		{id: "7", displayName: "Семён Семёныч", login: "ssemen", email: "ssemen@example.com", firstname: "", lastname: ""},
		{id: "8", displayName: "Олег Китаич", login: "olegk", email: "olegk@example.com", firstname: "", lastname: ""},
		{id: "9", displayName: "Василиса Александровна", login: "vasilisa",email: "vasilisa@example.com", firstname: "", lastname: ""},
		{id: "10", displayName: "Кристина Николаевна", login: "krisn", email: "krisn@example.com", firstname: "", lastname: ""},
		{id: "11", displayName: "Екатерина Сергеевна", login: "ekaserg", email: "ekaserg@example.com", firstname: "", lastname: ""},
		{id: "12", displayName: "Николай Николаевич", login: "niknik", email: "niknik@example.com", firstname: "", lastname: ""},
		{id: "13", displayName: "Олег Сидорович", login: "olegsid", email: "olegsid@example.com", firstname: "", lastname: ""},
		{id: "14", displayName: "Василий Семёнович", login: "vassem", email: "vassem@example.com", firstname: "", lastname: ""},
		{id: "15", displayName: "Елена Александровна", login: "elenaleks", email: "elenaleks@example.com", firstname: "", lastname: ""},
	];

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

	getById(userId: string): Observable<User> {
		const user = this._mockDataForSpaceDetailPage.find(u => u.id === userId);
		return of(user).pipe(delay(2000));
	}

	getByLogin(login: string): Observable<User> {
		const user = this._mockDataForSpaceDetailPage.find(u => u.login === login);
		return of(user).pipe(delay(2000));
	}

	fillUserFields<T extends UserFields & {userId: string}>(items: T[]) {
		const ids = new Set<string>();
		items.forEach(user => ids.add(user.userId));
		for (const id of ids) {
			this.getById(id).subscribe(user => {
				for (const item of items) {
					if (item.userId !== user.id)
						continue;
					item.displayName = user.displayName;
					item.login = user.login;
					item.email = user.email;
				}
			});
		}
	}
}
