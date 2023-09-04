export class Workspace {
	id: string;
	name: string;
	title: string;
}

export interface WorkspaceListResponse {
	items: Workspace[];
}
