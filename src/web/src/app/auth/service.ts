import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, concatMap, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {RefreshTokenResponse} from './model';
import {User} from '../user/model';
import {UserService} from '../user/service';

import jwtDecode, { JwtPayload } from "jwt-decode";
import { SpaceUserAddinitionalInfo } from '../space/model';

@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject(1);
	private _token: string;
	private _timeoutHandle: any = null;

	private _apiUrl = '/api/auth/';

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

	get token(): string {
		return this._token;
	}

	constructor(
		private http: HttpClient,
		private user: UserService,
	) {}

	private stopRefreshTimer() {
		if (this._timeoutHandle === null)
			return;
		clearTimeout(this._timeoutHandle);
		this._timeoutHandle = null;
	}

	private startRefreshTimer() {
		const decoded = jwtDecode<JwtPayload>(this._token);
		let msecs = decoded.exp * 1000 - new Date().getTime() - 30000;
		if (msecs <= 1000)
			msecs = 1000;

		this.stopRefreshTimer();
		this._timeoutHandle = setTimeout(() => this.refreshToken(), msecs);
	}

	CheckAndLogout() {
		this._token = localStorage.getItem('first');
		this.goToLogout();
	}

	Check(): Observable<boolean> {
		if (this._isChecked)
			return this.isAuthorized();
		this._isChecked = true;

		const token = localStorage.getItem('first');
		if (!token) {
			this.goToLogout();
			return of(false);
		}

		if (this.isTokenExpired(token)) {
			this._token = token;
			this.refreshToken();
			return this.isAuthorized();
		}

		return this.setToken(token);
	}

	private isTokenExpired(token: string): boolean {
		const decoded = jwtDecode<JwtPayload>(token);
		return new Date().getTime() > decoded.exp * 1000;
	}

	private setToken(token: string): Observable<boolean> {
		if (!token.length) {
			const err = new Error('Token is empty');
			console.error(err.message);

			this.goToLogout();
			return throwError(() => err);
		}

		this._token = token;
		return this.user.FetchInfo().pipe(
			concatMap(res => {
				this._isAuthorized.next(true);
				this.startRefreshTimer();
				return of(res);
			}),
			catchError(err => {
				this.goToLogout();
				return throwError(() => err);
			})
		);
	}

	SaveToken(token: string): Observable<boolean> {
		this._isChecked = false;
		localStorage.setItem('first', token);
		return this.Check();
	}

	isAuthorized(): Observable<boolean> {
		return this._isAuthorized.asObservable();
	}

	private refreshToken() {
		this.stopRefreshTimer();

		if (!this._token) {
			this.goToLogout();
			return;
		}

		this.http.post<RefreshTokenResponse>(this._apiUrl + 'token/refresh', null).pipe(
			catchError(err => {
				console.error("Refresh token error:", err);
				this.goToLogout();
				return of();
			})
		).subscribe(resp => this.SaveToken((<RefreshTokenResponse>resp).token));
	}

	goToLogin(): void {
		this.stopRefreshTimer();
		localStorage.removeItem('first');
		this._token = null;
		this._isAuthorized.next(false);
		window.location.href = window.location.origin + this._apiUrl + "login";
	}

	goToLogout(): void {
		if (!this._token || this.isTokenExpired(this._token)) {
			this.goToLogin();
			return;
		}

		this.stopRefreshTimer();
		window.location.href = window.location.origin + this._apiUrl + 'logout?token=' + this._token;
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
