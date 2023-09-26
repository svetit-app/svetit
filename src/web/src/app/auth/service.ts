import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {catchError, concatMap, switchMap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {RefreshTokenResponse} from './model';
import {User} from '../user/model';
import {UserService} from '../user/service';
import { SpaceService } from '../space/service';

import jwtDecode, { JwtPayload } from "jwt-decode";

@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject();
	private _token: string;
	private _timeoutHandle: any = null;

	private _apiUrl = '/api/auth/';

	get token(): string {
		return this._token;
	}

	constructor(
		private http: HttpClient,
		private user: UserService,
		private space: SpaceService,
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

	Check() { // TODO: check once
		if (this._isChecked && !!this.user.info)
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
				return throwError(err);
			})
		);
	}

	SaveToken(token: string): Observable<boolean> {
		return this.setToken(token).pipe(
			concatMap(res => {
				localStorage.setItem('first', token);
				this.Check();
				return this.space.Fetch();
			})
		);
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
}
