import {User} from './auth/model';

export interface PaginatorApi<T> {
	results: T[]; // Массив данных
	count: number; // Общее кол-во элементов в базе
}

export interface Paging<T> {
	list: T[]; // Массив данных
	total: number; // Общее кол-во элементов в базе
}

export class Named_Object {
	id: number;
	name: string;
}

export class Auth_Group extends Named_Object {}
export class Scheme_Group extends Named_Object {}
export class Scheme_Group_Member extends Named_Object {}

export enum Group_User_Roles {
	ADMIN = 'admin',
	USER = 'user',
}

export type UserHeader = Pick<User, 'id' | 'firstname' | 'lastname' | 'login' | 'displayName'>;
export type UserHeaderWithRole = UserHeader & { role: Group_User_Roles };

export enum Connection_State {
	CS_SERVER_DOWN,
	CS_DISCONNECTED,
	CS_DISCONNECTED_JUST_NOW,
	CS_CONNECTED_JUST_NOW,
	CS_CONNECTED_SYNC_TIMEOUT,
	CS_CONNECTED,

	CS_CONNECTED_WITH_LOSSES = 0x40,
	CS_CONNECTED_MODIFIED = 0x80
}

export enum Sync_Direction {
	NODE_TO_PROJECT,
	PROJECT_TO_NODE,
}

export interface Scheme_Message<T = string | number> {
		status: T;
		text: string;
		where: string;

		section?: string;
		section_id?: number;
		group?: string;
		group_id?: number;
}

export class Scheme {
	id: number;
	name: string;				// Имя прокта латиницей и без спец. символов, используется как имя базы данных
	device: string;			// UUID проекта
	last_usage: string;
	title: string;			// Отображаемое имя проекта
	city: number | null;
	company: number | null;
	description: string;
	version: string;
	messages: Scheme_Message[];
	parent: number;
	connection: number;
	mod_state: boolean;
	loses_state: boolean;
	status_checked: boolean;
	connect_state: Connection_State;
}

export class Project {
	id: number;
	space_id: string;
	key: string;
	name: string;
	description: string;
	changed_at: number;
	sync: Sync_Direction;
}
