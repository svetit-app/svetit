import {inject} from '@angular/core';
import {SpaceService} from "./service";

export const spaceGuard = () => {
	const wsp = inject(SpaceService);
	return wsp.isInitialized();
};
