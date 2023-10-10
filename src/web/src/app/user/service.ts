import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {NavigationExtras, Router} from '@angular/router';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User} from './model';
import {SpaceService} from '../space/service';
import { SpaceUserAddinitionalInfo } from '../space/model';

@Injectable()
export class UserService {
	private _info: User = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	private _mockDataForSpaceDetailPage: SpaceUserAddinitionalInfo[] = [
		{name: "Петр Петрович", userId: "petya", email: "petya@example.com"},
		{name: "Василий Иванович", userId: "vasya", email: "vasya@example.com"},
		{name: "Николай Александрович", userId: "kolya", email: "kolya@example.com"},
		{name: "Ольга Ивановна", userId: "olgaiv", email: "olgaiv@example.com"},
		{name: "Екатерина Петровна", userId: "ekapet", email: "ekapet@example.com"},
		{name: "Сергей Николаевич", userId: "sergnik", email: "sergnik@example.com"},
		{name: "Семён Семёныч", userId: "ssemen", email: "ssemen@example.com"},
		{name: "Олег Китаич", userId: "olegk", email: "olegk@example.com"},
		{name: "Василиса Александровна", userId: "vasilisa",email: "vasilisa@example.com"},
		{name: "Кристина Николаевна", userId: "krisn", email: "krisn@example.com"},
		{name: "Екатерина Сергеевна", userId: "ekaserg", email: "ekaserg@example.com"},
		{name: "Николай Николаевич", userId: "niknik", email: "niknik@example.com"},
		{name: "Олег Сидорович", userId: "olegsid", email: "olegsid@example.com"},
		{name: "Василий Семёнович", userId: "vassem", email: "vassem@example.com"},
		{name: "Елена Александровна", userId: "elenaleks", email: "elenaleks@example.com"},
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

	getUserEmailById(userId: string) {
		let userAddInfo = this._mockDataForSpaceDetailPage.find(u => u.userId === userId);
		return userAddInfo?.email;
	}

	getUserNameById(userId: string) {
		let userAddInfo = this._mockDataForSpaceDetailPage.find(u => u.userId === userId);
		return userAddInfo?.name;
	}
}
