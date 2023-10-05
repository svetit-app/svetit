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

export interface SpaceInvitation {
	id: string,
	spaceId: string;
	creatorId: string;
	userId: string;
	role: string;
	createdAt: Date;
}

export interface SpaceLink {
	id: string;
	spaceId: string;
	creatorId: string;
	name: string;
	createdAt: Date;
	expiredAt: Date;
}