import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of} from 'rxjs';
import {catchError} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {Tokens} from './model';
import {User} from '../users/model';
import {UsersService} from '../users/service';

import jwtDecode, { JwtPayload } from "jwt-decode";

@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject();
	private _token: Tokens;
	private _timeoutHandle: any;

	private _authUrl = '/api/auth/';

	get tokens(): Tokens {
		return this._token;
	}

	constructor(
		private http: HttpClient,
		private users: UsersService,
	) {}

	private startRefreshTimer() {
		const decoded = jwtDecode<JwtPayload>(this._token.access);
		let msecs = decoded.exp * 1000 - new Date().getTime() - 15000;
		if (msecs <= 1000)
			msecs = 1000;

		clearTimeout(this._timeoutHandle);
		this._timeoutHandle = setTimeout(() => this.refreshToken(), msecs);
	}

	Check() { // TODO: check once
		if (this._isChecked && !!this.users.user)
			return;
		this._isChecked = true;

		const json = localStorage.getItem('first');
		const tokens = JSON.parse(json || '{}') as Tokens;
		if (!json || !tokens) {
			this.goToLogout();
			return;
		}

		if (this.isTokenExpired(tokens.access)) {
			this.refreshToken(tokens);
			return;
		}

		const user = this.parseUser(tokens);
		if (!user || !this.setTokens(tokens)) {
			this.goToLogout();
			return;
		}

		this.users.SetUser(user);
	}

	private isTokenExpired(token: string): boolean {
		const decoded = jwtDecode<JwtPayload>(token);
		return new Date().getTime() > decoded.exp * 1000;
	}

	private setTokens(token: Tokens): boolean {
		if (!token.access.length || !token.refresh.length || !token.logout.length) {
			console.error("Tokens empty", token);
			return false;
		}
		this._token = token;
		this._isAuthorized.next(true);
		this.startRefreshTimer();
		return true;
	}

	SaveTokens(tokens: Tokens): boolean {
		const user = this.parseUser(tokens);
		if (!user || !this.setTokens(tokens)) {
			this.goToLogout();
			return false;
		}
		localStorage.setItem('first', JSON.stringify(tokens));
		this.Check();
		return true;
	}

	private parseUser(tokens: Tokens): User {
		const decoded = jwtDecode<JwtPayload>(tokens.access);
		const data = <any>decoded;
		return {
			id: decoded.sub || '',
			username: data.preferred_username,
			first_name: data.given_name,
			last_name: data.family_name,
			email: data.email,
			token: tokens,
		} as User;
	}

	isAuthorized(): Observable<boolean> {
		return this._isAuthorized.asObservable();
	}

	private refreshToken(token: Tokens = undefined) {
		clearTimeout(this._timeoutHandle);

		if (!token)
			token = this._token;

		if (!token || !token.refresh || this.isTokenExpired(token.refresh)) {
			this.goToLogout();
			return;
		}

		this.http.post<Tokens>(this._authUrl + 'token/refresh', { token: token.refresh }).pipe(
			catchError(err => {
				console.error("Refresh token error:", err);
				this.goToLogout();
				return of();
			})
		).subscribe(token => this.SaveTokens(token as Tokens));
	}

	goToLogin(): void {
		window.location.href = window.location.origin + this._authUrl + "login";
	}

	goToLogout(): void {
		clearTimeout(this._timeoutHandle);
		localStorage.removeItem('first');
		this._isAuthorized.next(false);

		if (!this._token || this.isTokenExpired(this._token.logout)) {
			this.goToLogin();
			return;
		}

		this._token = null;
		const token = encodeURIComponent(this._token.logout);
		window.location.href = window.location.origin + this._authUrl + 'logout?token=' + token;
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
