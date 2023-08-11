import {inject} from '@angular/core';
import {AuthService} from "./service";

export const authGuard = () => {
	const auth = inject(AuthService);
	auth.Check();
	return auth.isAuthorized();
};
