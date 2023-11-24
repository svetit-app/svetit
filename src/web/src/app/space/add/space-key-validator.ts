import { Directive } from '@angular/core';
import { AbstractControl, AsyncValidator, NG_ASYNC_VALIDATORS, ValidationErrors } from '@angular/forms';
import { map, catchError } from 'rxjs/operators';
import { Observable, throwError, of } from 'rxjs';
import { HttpErrorResponse } from '@angular/common/http';

import { SpaceService } from '../service';

@Directive({
	selector: '[spaceKeyExists][formControlName],[spaceKeyExists][formControl],[spaceKeyExists][ngModel]',
	providers: [{ provide: NG_ASYNC_VALIDATORS, useExisting: SpaceKeyValidatorDirective, multi: true }]
})
export class SpaceKeyValidatorDirective implements AsyncValidator {
	constructor(private space: SpaceService) {}

	validate(control: AbstractControl): Promise<ValidationErrors | null> | Observable<ValidationErrors | null> {
		return this.space.isExists(control.value)
			.pipe(
				map(res => {
					if (res.status == 200) {
						return { 'keyExists': true };
					}
				}),
				catchError(this.handleError),
			);
	}

	private handleError(error: HttpErrorResponse) {
		if (error.status === 0) {
			// A client-side or network error occurred. Handle it accordingly.
			console.error('An error occurred:', error.error);
		} else {
			// The backend returned an unsuccessful response code.
			// The response body may contain clues as to what went wrong.
			console.error(`Backend returned code ${error.status}, body was: `, error.error);
			// if 404 was returned it means that checked space is not exist, so return null
			if (error.status == 404)
				return of(null);
		}
		// Return an observable with a user-facing error message.
		return throwError(() => new Error('Something bad happened; please try again later.'));
	}
}