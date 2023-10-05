export class Space {
	id: string;
	name: string;
	title: string;
}

export interface SpaceListResponse {
	items: Space[];
}

export interface SpaceInterface {
	id: string;
	name: string;
	key: string;
	requestsAllowed: boolean;
	createdAt: Date;
}
