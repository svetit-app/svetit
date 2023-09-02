export interface UserInfo {
	defaultUnion: boolean;
	unions: string[];
	permissions: string[];
}

export class User {
	id: string;
	name: string;
	username: string;
	firstname: string;
	lastname: string;
	email: string;
}
