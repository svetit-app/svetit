import { Component, OnInit, Inject, ViewChild, ElementRef } from '@angular/core';
import { DOCUMENT } from '@angular/common';
import { MatPaginator} from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { Observable} from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';

import { Space, SpaceLink, SpaceFields} from '../model';
import { UserFields } from '../../user/model';
import { User } from '../../user/model';
import { SpaceService } from '../service';
import { UserService } from '../../user/service';
import { SpaceInvitationListComponent } from '../invitation-list/component';

type SpaceLinkDetail = SpaceLink & SpaceFields;

@Component({
	selector: 'app-space-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceListComponent implements OnInit {
	linkForm: FormGroup;
	isLinkFormHidden: boolean = true;
	linkFormSpaceId: string;
	linkFormSpaceName: string;

	pageSize = {
		invitations: 7,
		links: 7,
		spaces: 7
	};

	// userId текущего залогиненного юзера
	currentUserId: string;

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/space/link/";

	spacesTotal: number;
	linksTotal: number;

	links: SpaceLinkDetail[] = [];
	spaces: Space[] = [];

	@ViewChild('invitationList') invitationList: SpaceInvitationListComponent;
	@ViewChild('invitationList', { read: ElementRef }) scrollToInvitationList: ElementRef<HTMLElement>;

	@ViewChild('linksPaginator') linksPaginator: MatPaginator;
	@ViewChild('spacesPaginator') spacesPaginator: MatPaginator;
	@ViewChild('scrollToLinkForm') scrollToLinkForm: ElementRef;

	constructor(
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._initLinkForm();
	}

	ngOnInit() {
		this.currentUserId = this.user.info.id;

		const pageSizeStr = localStorage.getItem('spaceListPageSize');
		const pageSize = JSON.parse(pageSizeStr);
		if (pageSize?.invitations && pageSize?.links && pageSize?.spaces) {
			this.pageSize = pageSize;
		}

		this.getLinks(this.pageSize.links, 0);
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

	getLinks(limit: number, page: number) {
		this.savePageSize("links", limit);
		this.space.getLinkList(limit, page)
			.subscribe(res => {
				this.links = res.results as SpaceLinkDetail[];
				this.linksTotal = res.count;
				this.space.fillFields(this.links);
			});
	}

	onLinkCopyBtn(link: SpaceLink) {
		let copyToClipboard = this.document.createElement('textarea');
		copyToClipboard.style.position = 'fixed';
		copyToClipboard.style.left = '0';
		copyToClipboard.style.top = '0';
		copyToClipboard.style.opacity = '0';
		copyToClipboard.value = this.document.location.origin + this.linksURL + link.id;
		document.body.appendChild(copyToClipboard);
		copyToClipboard.focus();
		copyToClipboard.select();
		document.execCommand('copy');
		document.body.removeChild(copyToClipboard);
	}

	onLinkDelBtn(link: SpaceLink) {
		this.space.delLinkById(link.id)
			.subscribe(_ => {
				if (this.linksPaginator.pageIndex == 0) {
					this.getLinks(this.pageSize.links, 0);
				} else {
					this.linksPaginator.firstPage();
				}
			});
	}

	onSpaceDelBtn(space: Space) {
		this.space.delSpaceById(space.id)
			.subscribe(_ => {
				if (this.spacesPaginator.pageIndex == 0) {
					this.getSpaces(this.pageSize.spaces, 0);
				} else {
					this.spacesPaginator.firstPage();
				}
			});
	}

	private _initLinkForm() {
		this.linkForm = this.fb.group({
			name: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			expiredAt: [null, [Validators.required]],
		});
	}

	onSpaceInvitationAddUser(space: Space) {
		this.invitationList.onAddUser(space);
		this.scrollToInvitationList.nativeElement.scrollIntoView();
	}

	onSpaceAddLink(space: Space) {
		this.isLinkFormHidden = false;
		this.linkFormSpaceId = space.id;
		this.linkFormSpaceName = space.name;
		this.scrollToLinkForm.nativeElement.scrollIntoView();
	}

	onLinkFormCloseBtn() {
		this.isLinkFormHidden = true;
		this.linkFormSpaceName = "";
		this.linkForm.reset();
	}

	onSubmitLink(): void {
		if (this.linkForm.invalid) {
			return;
		}
		this.space.createLink(
			this.linkFormSpaceId,
			this.currentUserId,
			this.linkForm.value.name,
			this.linkForm.value.expiredAt
		).subscribe(_ => {
			this.linkForm.reset();
			this.isLinkFormHidden = true;
			if (this.linksPaginator.pageIndex == 0) {
				this.getLinks(this.pageSize.links, 0);
			} else {
				this.linksPaginator.firstPage();
			}
		});
	}

	savePageSize(id: string, limit: number) {
		if (this.pageSize[id] == limit)
			return;

		this.pageSize[id] = limit;
		localStorage.setItem('spaceListPageSize', JSON.stringify(this.pageSize));
	}
}
