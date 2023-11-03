import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { MatPaginator } from '@angular/material/paginator';

import { Space } from '../model';
import { SpaceService } from '../service';
import { SpaceInvitationListComponent } from '../invitation-list/component';
import { SpaceLinkListComponent } from '../link-list/component';

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css']
})
export class SpaceListComponent implements OnInit {
	pageSize = {
		invitations: 7,
		links: 7,
		spaces: 7
	};

	spacesTotal: number;

	spaces: Space[] = [];

	@ViewChild('invitationList') invitationList: SpaceInvitationListComponent;
	@ViewChild('invitationList', { read: ElementRef }) scrollToInvitationList: ElementRef<HTMLElement>;

	@ViewChild('linkList') linkList: SpaceLinkListComponent;
	@ViewChild('linkList', { read: ElementRef }) scrollToLinkList: ElementRef<HTMLElement>;

	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;

	constructor(private space: SpaceService) {}

	ngOnInit() {
		const pageSizeStr = localStorage.getItem('spaceListPageSize');
		const pageSize = JSON.parse(pageSizeStr);
		if (pageSize?.invitations && pageSize?.links && pageSize?.spaces) {
			this.pageSize = pageSize;
		}

		this.getSpaces(this.pageSize.spaces, 0);
	}

	getSpaces(limit: number, page: number) {
		this.savePageSize("spaces", limit);
		this.space.getList(limit, page)
			.subscribe(res => {
				this.spaces = res.results;
				this.spacesTotal = res.count;
			});
	}

	onSpaceDelBtn(space: Space) {
		this.space.delById(space.id)
			.subscribe(_ => {
				if (this.spacesPaginator.pageIndex == 0) {
					this.getSpaces(this.pageSize.spaces, 0);
				} else {
					this.spacesPaginator.firstPage();
				}
			});
	}

	onSpaceInvitationAddUser(space: Space) {
		this.invitationList.onAdd(space);
		this.scrollToInvitationList.nativeElement.scrollIntoView();
	}

	onSpaceAddLink(space: Space) {
		this.linkList.onAdd(space);
		this.scrollToLinkList.nativeElement.scrollIntoView();
	}

	savePageSize(id: string, limit: number) {
		if (this.pageSize[id] == limit)
			return;

		this.pageSize[id] = limit;
		localStorage.setItem('spaceListPageSize', JSON.stringify(this.pageSize));
	}
}
