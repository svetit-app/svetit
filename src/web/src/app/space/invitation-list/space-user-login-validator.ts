import { Directive } from '@angular/core';
import { AbstractControl, AsyncValidator, NG_ASYNC_VALIDATORS, ValidationErrors } from '@angular/forms';
import { map } from 'rxjs/operators';
import { Observable } from 'rxjs';

import { UserService } from '../../user/service';

@Directive({
	selector: '[spaceUserExists][formControlName],[spaceUserExists][formControl],[spaceUserExists][ngModel]',
	providers: [{ provide: NG_ASYNC_VALIDATORS, useExisting: SpaceUserLoginValidatorDirective, multi: true }]
})
export class SpaceUserLoginValidatorDirective implements AsyncValidator {
	constructor(private user: UserService) {}

	validate(control: AbstractControl): Promise<ValidationErrors | null> | Observable<ValidationErrors | null> {
		if (control.value.hasOwnProperty('login')) {
			return this.user.isExists(control.value.login)
			.pipe(map(res => {
				if (res) {
					return null;
				} else {
					return {'userNotExists': true};
				}
			}));
		} else {
			return this.user.isExists(control.value)
			.pipe(map(res => {
				if (res) {
					return null;
				} else {
					return {'userNotExists': true};
				}
			}));
		}

	}
}
