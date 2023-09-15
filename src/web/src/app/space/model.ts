export class Space {
	id: string;
	name: string;
	title: string;
}

export interface SpaceListResponse {
	items: Space[];
}
