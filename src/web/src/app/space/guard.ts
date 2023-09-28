import {inject} from '@angular/core';
import {SpaceService} from "./service";

export const spaceGuard = () => {
	const space = inject(SpaceService);
	return space.Check();
};
