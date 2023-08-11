export interface UserInfo {
	defaultUnion: boolean;
	unions: string[];
	permissions: string[];
}

export class User {
	id: string;
	username: string;
	first_name: string;
	last_name: string;
	email: string;
}
