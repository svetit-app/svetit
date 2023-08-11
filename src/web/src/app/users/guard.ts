import {inject} from '@angular/core';
import {UsersService} from "./service";

export const usersUnionGuard = () => {
	const users = inject(UsersService);
	return users.isInitialized();
};
