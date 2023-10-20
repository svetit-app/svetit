import { Directive } from '@angular/core';
import { AbstractControl, AsyncValidator, NG_ASYNC_VALIDATORS, ValidationErrors } from '@angular/forms';
import { map } from 'rxjs/operators';
import { Observable } from 'rxjs';

import { SpaceService } from '../service';

@Directive({
	selector: '[spaceKeyExists][formControlName],[spaceKeyExists][formControl],[spaceKeyExists][ngModel]',
	providers: [{ provide: NG_ASYNC_VALIDATORS, useExisting: SpaceKeyValidatorDirective, multi: true }]
})
export class SpaceKeyValidatorDirective implements AsyncValidator {
	constructor(private space: SpaceService) {}

	validate(control: AbstractControl): Promise<ValidationErrors | null> | Observable<ValidationErrors | null> {
		return this.space.isExists(control.value)
			.pipe(map(res => {
				return res ? { 'keyExists': true} : null;
			}));
	}
}
