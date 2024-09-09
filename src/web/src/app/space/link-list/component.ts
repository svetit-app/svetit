import { Component, OnInit, Inject, ViewChild, ElementRef, Input, Output, EventEmitter } from '@angular/core';
import { DOCUMENT } from '@angular/common';
import { MatPaginator} from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { MatPaginatorModule } from '@angular/material/paginator';

import { SpaceLink, SpaceFields} from '../model';
import { SpaceService } from '../service';
import { AuthService } from '../../auth/service';

import { Link, Space } from '../../api';

type Detail = Link & SpaceFields;

@Component({
	selector: 'app-space-link-list',
	standalone: true,
	imports: [
		MatPaginatorModule,
	],
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css']
})
export class SpaceLinkListComponent implements OnInit {
	form: FormGroup;
	isFormHidden: boolean = true;
	formSpaceId: string;
	formSpaceName: string;

	total: number;

	@Input() pageSize: number = 7;
	@Output() onPageSize = new EventEmitter<number>();

	// userId текущего залогиненного юзера
	currentUserId: string;

	_space: Space = null;
	@Input()
	set currentSpace(value: Space) {
		this._space = value;
		this.getItems(this.pageSize, 0);
	}
	get currentSpace() {
		return this._space;
	}

	// относительный адрес для ссылок-приглашений
	linksURL: string = "/space/link/";

	items: Detail[] = [];

	@ViewChild('paginator') paginator: MatPaginator;

	constructor(
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private auth: AuthService,
	) {
		this._initForm();
	}

	ngOnInit() {
		this.currentUserId = this.auth.user.id;
		this.getItems(this.pageSize, 0);
	}

	getItems(limit: number, page: number) {
		if (this.pageSize != limit) {
			this.pageSize = limit;
			this.onPageSize.emit(limit);
		}

		// Если space уже установлен, но ещё не заполнены поля (не получен объект с сервера)
		if (this._space && !this._space.id)
			return;

		this.space.getLinkList(limit, page, this._space?.id)
			.subscribe(res => {
				this.items = res.list as Detail[];
				this.total = res.total;

				if (!this._space?.id)
					this.space.fillFields(this.items);
			});
	}

	onCopyBtn(link: SpaceLink) {
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

	onDelBtn(link: Detail) {
		this.space.delLinkById(link.id)
			.subscribe(_ => {
				if (this.paginator.pageIndex == 0) {
					this.getItems(this.pageSize, 0);
				} else {
					this.paginator.firstPage();
				}
			});
	}

	private _initForm() {
		this.form = this.fb.group({
			name: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			]],
			expiredAt: [null, [Validators.required]],
		});
	}

	onAdd(space: Space) {
		this.isFormHidden = false;
		this.formSpaceId = space.id;
		this.formSpaceName = space.name;
	}

	onFormCloseBtn() {
		this.isFormHidden = true;
		this.formSpaceName = "";
		this.form.reset();
	}

	onSubmit(): void {
		if (this.form.invalid) {
			return;
		}
		this.space.createLink(
			this.formSpaceId,
			this.form.value.name,
			this.form.value.expiredAt
		).subscribe(_ => {
			this.form.reset();
			this.isFormHidden = true;
			if (this.paginator.pageIndex == 0) {
				this.getItems(this.pageSize, 0);
			} else {
				this.paginator.firstPage();
			}
		});
	}
}
