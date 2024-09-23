import { Injectable, inject } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, pipe, of } from 'rxjs';
import { UnaryFunction } from 'rxjs/interfaces';
import { catchError, tap } from 'rxjs/operators';

import { MessageService } from './message.service';

const httpOptions = {
	headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class ISchemeService {
	protected http = inject(HttpClient);
	private messageService = inject(MessageService);


	protected apiUrl = '/api/';

	protected log(message: string) {
		this.messageService.add('SchemeService: ' + message);
	}

	protected piping<T> (succes_string: string, operation: string, result?: T): UnaryFunction<Observable<T>, Observable<T>> {
		return pipe(
			tap(() => this.log(succes_string)),
			catchError(this.handleError(operation, result))
		);
	}

	protected httpGet<T> (url_postfix: string): Observable<T> {
		return this.http.get<T>(this.apiUrl + url_postfix);
	}

	public getPiped<T> (url_postfix: string, succes_string: string, operation: string, result?: T): Observable<T> {
		return this.httpGet<T>(url_postfix).pipe(this.piping<T>(succes_string, operation, result));
	}

	public put<T> (url_postfix: string, obj: T): Observable<T> {
		return this.http.put<T>(this.apiUrl + url_postfix, obj, httpOptions);
	}

	public putPiped<T> (url_postfix: string, obj: T, succes_string: string, operation: string, result?: T): Observable<T> {
		return this.put<T>(url_postfix, obj).pipe(this.piping<T>(succes_string, operation, result));
	}

	protected post<T> (url_postfix: string, obj: T): Observable<T> {
		return this.http.post<T>(this.apiUrl + url_postfix, obj, httpOptions);
	}

	public postPiped<T> (url_postfix: string, obj: T, succes_string: string, operation: string, result?: T): Observable<T> {
		return this.post<T>(url_postfix, obj).pipe(this.piping<T>(succes_string, operation, result));
	}

	protected patch<T> (url_postfix: string, obj: T): Observable<T> {
		return this.http.patch<T>(this.apiUrl + url_postfix, obj, httpOptions);
	}

	public patchPiped<T> (url_postfix: string, obj: T, succes_string: string, operation: string, result?: T): Observable<T> {
		return this.patch<T>(url_postfix, obj).pipe(this.piping<T>(succes_string, operation, result));
	}

	protected delete<T> (url_postfix: string): Observable<T> {
		return this.http.delete<T>(this.apiUrl + url_postfix, httpOptions);
	}

	public deletePiped<T> (url_postfix: string, succes_string: string, operation: string, result?: T): Observable<T> {
		return this.delete<T>(url_postfix).pipe(this.piping<T>(succes_string, operation, result));
	}

	/**
	 * Handle Http operation that failed.
	 * Let the app continue.
	 * @param operation - name of the operation that failed
	 * @param result - optional value to return as the observable result
	 */
	protected handleError<T> (operation = 'operation', result?: T) {
		return (error: any): Observable<T> => {

			// TODO: send the error to remote logging infrastructure
			console.error(error); // log to console instead

			// TODO: better job of transforming error for user consumption
			this.log(`${operation} failed: ${error.message}`);

			// Let the app keep running by returning an empty result.
			return of(result as T);
		};
	}
}
