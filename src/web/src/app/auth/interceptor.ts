import { Injectable } from '@angular/core';
import { Router } from '@angular/router';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { throwError } from 'rxjs';
import 'rxjs/add/operator/catch';

import {AuthService} from "./service";

@Injectable()
export class AuthInterceptor implements HttpInterceptor {

	constructor(
		private auth: AuthService,
		private router: Router
	) {}
	
	intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
		return next.handle(request)
			.catch((error, caught) => {
				if (error.status === 401) {
					// logout users, redirect to login page
					this.auth.GoToLogin();
					return throwError(error);
				}

				return throwError(error);
			}) as any;
	}
}
