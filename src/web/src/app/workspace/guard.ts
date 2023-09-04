import {inject} from '@angular/core';
import {WorkspaceService} from "./service";

export const workspaceGuard = () => {
	const wsp = inject(WorkspaceService);
	return wsp.isInitialized();
};
