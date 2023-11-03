import {inject} from '@angular/core';
import {SpaceService} from "./service";
import { of } from 'rxjs';
import { switchMap } from 'rxjs/operators';

export const spaceGuard = () => {
	const space = inject(SpaceService);
	return space.Check().pipe(
		switchMap(_ => of(true))
	);
};
