import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of} from 'rxjs';
import {catchError} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import {Session} from './model';
import {User} from '../users/model';
import {UsersService} from '../users/service';

import jwtDecode, { JwtPayload } from "jwt-decode";

@Injectable()
export class AuthService {
	private _isChecked = false;
	private _isAuthorized: ReplaySubject<boolean> = new ReplaySubject();
	private _session: Session;
	private _timeoutHandle: any;

	private _authUrl = '/api/auth/';

	get session(): Session {
		return this._session;
	}

	constructor(
		private http: HttpClient,
		private users: UsersService,
	) {}

	/*private startRefreshTimer() {
		const decoded = jwtDecode<JwtPayload>(this._token.access);
		let msecs = decoded.exp * 1000 - new Date().getTime() - 15000;
		if (msecs <= 1000)
			msecs = 1000;

		clearTimeout(this._timeoutHandle);
		this._timeoutHandle = setTimeout(() => this.refreshToken(), msecs);
	}*/

	Check() { // TODO: check once
		if (this._isChecked && !!this.users.user)
			return;
		this._isChecked = true;

		const json = localStorage.getItem('first');
		const session = JSON.parse(json || '{}') as Session;
		if (!json || !session) {
			this.goToLogout();
			return;
		}

		/*if (this.isTokenExpired(tokens.access)) {
			this.refreshToken(tokens);
			return;
		}*/

		const user = this.parseUser(session);
		if (!user || !this.setTokens(session)) {
			this.goToLogout();
			return;
		}

		this.users.SetUser(user);
	}

	private isTokenExpired(token: string): boolean {
		const decoded = jwtDecode<JwtPayload>(token);
		return new Date().getTime() > decoded.exp * 1000;
	}

	private setTokens(session: Session): boolean {
		if (!session.token.length) {
			console.error("Token is empty", session);
			return false;
		}
		this._session = session;
		this._isAuthorized.next(true);
		//this.startRefreshTimer();
		return true;
	}

	SaveSession(session: Session): boolean {
		const user = this.parseUser(session);
		if (!user || !this.setTokens(session)) {
			this.goToLogout();
			return false;
		}
		localStorage.setItem('first', JSON.stringify(session));
		this.Check();
		return true;
	}

	private parseUser(session: Session): User {
		const decoded = jwtDecode<JwtPayload>(session.token);
		const data = <any>decoded;
		return {
			id: data.sub || '',
			username: session.userLogin,
			first_name: session.username, // need to split username to first and last name
			last_name: session.username,
			email: "",
			token: session.token,
		} as User;
	}

	isAuthorized(): Observable<boolean> {
		return this._isAuthorized.asObservable();
	}

	/*private refreshToken(token: Tokens = undefined) {
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
	}*/

	goToLogin(): void {
		window.location.href = window.location.origin + this._authUrl + "login";
	}

	goToLogout(): void {
		clearTimeout(this._timeoutHandle);
		localStorage.removeItem('first');
		this._isAuthorized.next(false);

		if (!this._session || this.isTokenExpired(this._session.token)) {
			this.goToLogin();
			return;
		}

		this._session = null;
		//const session = encodeURIComponent(this._session.logout);
		//window.location.href = window.location.origin + this._authUrl + 'logout?token=' + token;
		// do we need to use some get param during logour as it was before with 
		window.location.href = window.location.origin + this._authUrl + 'logout';
	}

	logout() {
		// remove user from local storage to log user out
		localStorage.removeItem('first');
		if (this._session) {
			this._session = null;
			clearTimeout(this._timeoutHandle);
		}
	}
}
