import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {NavigationExtras, Router} from '@angular/router';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, switchMap, delay} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {User} from './model';
import {SpaceService} from '../space/service';

@Injectable()
export class UserService {
	private _info: User = null;
	private _permissions: string[] = [];

	private _apiUrl = '/api/user/';

	private _mockDataForSpaceDetailPage: User[] = [
		{id: "1", name: "Петр Петрович", username: "petya", email: "petya@example.com", firstname: "", lastname: ""},
		{id: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "Василий Иванович", username: "vasya", email: "vasya@example.com", firstname: "", lastname: ""},
		{id: "3", name: "Николай Александрович", username: "kolya", email: "kolya@example.com", firstname: "", lastname: ""},
		{id: "4", name: "Ольга Ивановна", username: "olgaiv", email: "olgaiv@example.com", firstname: "", lastname: ""},
		{id: "5", name: "Екатерина Петровна", username: "ekapet", email: "ekapet@example.com", firstname: "", lastname: ""},
		{id: "6", name: "Сергей Николаевич", username: "sergnik", email: "sergnik@example.com", firstname: "", lastname: ""},
		{id: "7", name: "Семён Семёныч", username: "ssemen", email: "ssemen@example.com", firstname: "", lastname: ""},
		{id: "8", name: "Олег Китаич", username: "olegk", email: "olegk@example.com", firstname: "", lastname: ""},
		{id: "9", name: "Василиса Александровна", username: "vasilisa",email: "vasilisa@example.com", firstname: "", lastname: ""},
		{id: "10", name: "Кристина Николаевна", username: "krisn", email: "krisn@example.com", firstname: "", lastname: ""},
		{id: "11", name: "Екатерина Сергеевна", username: "ekaserg", email: "ekaserg@example.com", firstname: "", lastname: ""},
		{id: "12", name: "Николай Николаевич", username: "niknik", email: "niknik@example.com", firstname: "", lastname: ""},
		{id: "13", name: "Олег Сидорович", username: "olegsid", email: "olegsid@example.com", firstname: "", lastname: ""},
		{id: "14", name: "Василий Семёнович", username: "vassem", email: "vassem@example.com", firstname: "", lastname: ""},
		{id: "15", name: "Елена Александровна", username: "elenaleks", email: "elenaleks@example.com", firstname: "", lastname: ""},
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
		return of(user);
			//.pipe(delay(2000));
	}

	getByUsername(username: string): Observable<User> {
		const user = this._mockDataForSpaceDetailPage.find(u => u.username === username);
		return of(user);
			//.pipe(delay(2000));
	}
}
