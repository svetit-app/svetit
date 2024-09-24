import { Injectable, inject } from '@angular/core';
import { Router } from '@angular/router';
import { HttpRequest, HttpHandler, HttpEvent, HttpInterceptor } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { catchError } from 'rxjs/operators';
import { throwError } from 'rxjs';

import {AuthService} from "./service";

@Injectable()
export class AuthInterceptor implements HttpInterceptor {
	private auth = inject(AuthService);
	private router = inject(Router);

	intercept(request: HttpRequest<any>, next: HttpHandler): Observable<HttpEvent<any>> {
		return next.handle(request).pipe(
			catchError(error => {
				if (error.status === 401) {
					// logout users, redirect to login page
					this.auth.GoToLogin();
					return throwError(error);
				}

				return throwError(error);
			})
		);
	}
}
