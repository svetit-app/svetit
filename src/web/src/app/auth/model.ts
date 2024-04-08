export class User {
	id: string;
	displayName: string;
	login: string;
	firstname: string;
	lastname: string;
	email: string;
}

export type UserFields = {
	userDisplayName: string;
	userLogin: string;
	userEmail: string;
}
