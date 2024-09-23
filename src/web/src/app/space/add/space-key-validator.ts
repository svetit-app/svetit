import { Directive, inject } from '@angular/core';
import { AbstractControl, AsyncValidator, NG_ASYNC_VALIDATORS, ValidationErrors } from '@angular/forms';
import { map, catchError, switchMap } from 'rxjs/operators';
import { Observable, throwError, of, timer} from 'rxjs';
import { HttpErrorResponse } from '@angular/common/http';

import { SpaceService } from '../service';

@Directive({
    selector: '[spaceKeyExists][formControlName],[spaceKeyExists][formControl],[spaceKeyExists][ngModel]',
    providers: [{ provide: NG_ASYNC_VALIDATORS, useExisting: SpaceKeyValidatorDirective, multi: true }],
    standalone: true
})
export class SpaceKeyValidatorDirective implements AsyncValidator {
	private space = inject(SpaceService);


	validate(control: AbstractControl): Promise<ValidationErrors | null> | Observable<ValidationErrors | null> {
		return timer(500).pipe(switchMap(() => {
			return this.space.isExists(control.value)
				.pipe(
					map(res => {
						if (res)
							return { 'keyExists': true };
						return null;
					})
				);
		}))
	}
}