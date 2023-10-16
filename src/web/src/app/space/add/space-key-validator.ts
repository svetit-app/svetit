import { Injectable } from '@angular/core';
import { AbstractControl, AsyncValidator, FormControl } from '@angular/forms';
import { map } from 'rxjs/operators';
import { SpaceService } from '../service';

@Injectable({
	providedIn: 'root',
})
export class SpaceKeyValidation implements AsyncValidator {

	constructor(private space: SpaceService) {}

	validate = (control: AbstractControl) => {

		const { value } = control;

		return this.space.isExists(value)
			.pipe(map(res => { 
				if (res) 
					return { 'keyExists': true}
			}));
  	};
}