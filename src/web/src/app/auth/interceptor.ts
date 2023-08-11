import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { throwError } from 'rxjs';
import 'rxjs/add/operator/catch';

import {AuthService} from "./service";
import {Tokens} from './model';

@Injectable()
export class AuthInterceptor implements HttpInterceptor {

	constructor(
		private auth: AuthService,
		private router: Router
	) {}

	private setTokenHeader(request: HttpRequest<any>, tokens: Tokens): HttpRequest<any> {
		if (tokens?.access?.length)
			return request.clone({
				setHeaders: {
					Authorization: `Bearer ${tokens.access}`
				}
			});
		return null as unknown as HttpRequest<any>;
	}
	
	intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
		// add authorization header with jwt token if available
		// let currentUser = JSON.parse(localStorage.getItem('currentUser'));
		const authReq = this.setTokenHeader(request, this.auth.tokens);
		if (authReq) {
			request = authReq;
		}
 
		return next.handle(request)
			.catch((error, caught) => {
				if (error.status === 401) {
					// logout users, redirect to login page
					this.auth.goToLogin();
					return throwError(error);
				}

				//return all others errors
				return throwError(error);
			}) as any;
	}
}
