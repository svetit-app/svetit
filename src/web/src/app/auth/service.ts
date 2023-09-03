import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, concatMap, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {RefreshTokenResponse} from './model';
import {User} from '../users/model';
import {UsersService} from '../users/service';

import jwtDecode, { JwtPayload } from "jwt-decode";

@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject();
	private _token: string;
	private _timeoutHandle: any;

	private _authUrl = '/api/auth/';

	get token(): string {
		return this._token;
	}

	constructor(
		private http: HttpClient,
		private users: UsersService,
	) {}

	private startRefreshTimer() {
		const decoded = jwtDecode<JwtPayload>(this._token);
		let msecs = decoded.exp * 1000 - new Date().getTime() - 30000;
		if (msecs <= 1000)
			msecs = 1000;

		clearTimeout(this._timeoutHandle);
		this._timeoutHandle = setTimeout(() => this.refreshToken(), msecs);
	}

	Check() { // TODO: check once
		if (this._isChecked && !!this.users.user)
			return;
		this._isChecked = true;

		const token = localStorage.getItem('first');
		if (!token) {
			this.goToLogout();
			return;
		}

		if (this.isTokenExpired(token)) {
			this._token = token;
			this.refreshToken();
			return;
		}

		this.setToken(token).subscribe();
	}

	private isTokenExpired(token: string): boolean {
		const decoded = jwtDecode<JwtPayload>(token);
		return new Date().getTime() > decoded.exp * 1000;
	}

	private setToken(token: string): Observable<void> {
		if (!token.length) {
			const err = new Error('Token is empty');
			console.error(err.message);
			
			this.goToLogout();
			return throwError(() => err);
		}

		this._token = token;
		return this.http.get('/api/user/info').pipe(
			switchMap(res => {
				this.users.SetUser(res as User);

				this._isAuthorized.next(true);
				this.startRefreshTimer();
				return of<void>();
			}),
			catchError(err => {
				this.goToLogout();
				return throwError(err);
			})
		);
	}

	SaveToken(token: string): Observable<void> {
		return this.setToken(token).pipe(
			concatMap(() => {
				localStorage.setItem('first', token);
				this.Check();
				return of<void>();
			})
		);
	}

	isAuthorized(): Observable<boolean> {
		return this._isAuthorized.asObservable();
	}

	private refreshToken() {
		clearTimeout(this._timeoutHandle);

		if (!this._token || this.isTokenExpired(this._token)) {
			this.goToLogout();
			return;
		}

		this.http.post<RefreshTokenResponse>(this._authUrl + 'token/refresh', null).pipe(
			catchError(err => {
				console.error("Refresh token error:", err);
				this.goToLogout();
				return of();
			})
		).subscribe(resp => this.SaveToken((<RefreshTokenResponse>resp).token));
	}

	goToLogin(): void {
		window.location.href = window.location.origin + this._authUrl + "login";
	}

	goToLogout(): void {
		clearTimeout(this._timeoutHandle);
		localStorage.removeItem('first');
		this._isAuthorized.next(false);

		if (!this._token || this.isTokenExpired(this._token)) {
			this.goToLogin();
			return;
		}

		this._token = null;
		window.location.href = window.location.origin + this._authUrl + 'logout';
	}

	logout() {
		// remove user from local storage to log user out
		localStorage.removeItem('first');
		if (this._token) {
			this._token = null;
			clearTimeout(this._timeoutHandle);
		}
	}
}
