import { Component, OnInit, Inject, ViewChild, Input, Output, EventEmitter } from '@angular/core';
import { DOCUMENT } from '@angular/common';
import { MatPaginator} from '@angular/material/paginator';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { Observable, of} from 'rxjs';
import { map, debounceTime, distinctUntilChanged, switchMap, tap, filter } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';

import { Space, SpaceInvitation, SpaceFields} from '../model';
import { UserFields } from '../../user/model';
import { User } from '../../user/model';
import { SpaceService } from '../service';
import { UserService } from '../../user/service';

enum INVITATION_TYPE {
	MY_REQUEST = 0,
	I_WAS_INVITED = 1,
	WE_INVITED = 2,
	WANTS_TO_JOIN = 3,
}

type Detail = SpaceInvitation & SpaceFields & UserFields & { type: INVITATION_TYPE };

@Component({
	selector: 'app-space-invitation-list',
	templateUrl: './component.html',
	styleUrls: ['./component.css', '../common.css']
})
export class SpaceInvitationListComponent implements OnInit {
	TYPE = INVITATION_TYPE;

	form: FormGroup;
	isFormHidden: boolean = true;
	formSpaceId: string;
	formSpaceName: string;
	formUser: User;

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

	items: Detail[] = [];

	users$: Observable<User[]>;
	hasUsers: boolean;

	@ViewChild('paginator') paginator: MatPaginator;

	constructor(
		@Inject(DOCUMENT) private document: any,
		private fb: FormBuilder,
		private space: SpaceService,
		private user: UserService,
	) {
		this._initForm();
	}

	ngOnInit() {
		this.currentUserId = this.user.info.id;

		this.users$ = this.form.controls['login'].valueChanges.pipe(
			tap(value => {
				this.formUser = this.hasUsers = undefined;

				// если передан объект, значит пользователь выбрал элемент из списка
				if (typeof value === "object") {
					this.formUser = value;
				}
			}),
			filter(value => typeof value === "string"), // ищем только если передана строка
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value =>  this.user.getList(10, 0, value).pipe(
				map(res => {
					this.hasUsers = res.results.length > 0;
					return res.results;
				}))
			)
		);

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

		this.space.getInvitationList(limit, page, this._space?.id)
			.subscribe(res => {
				this.items = res.list as Detail[];
				this.total = res.total;
				this.fillType();
				this.user.fillFields(this.items);

				if (!this._space?.id)
					this.space.fillFields(this.items);
			});
	}

	onDelBtn(item: Detail) {
		this.space.delInvitationById(item.id)
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
			login: ['', [
				Validators.required,
			]],
			role: ['', [
				Validators.required
			],],
		});
	}

	onAdd(space: Space) {
		this.formSpaceId = space.id;
		this.formSpaceName = space.name;
		this.isFormHidden = false;
	}

	onFormCloseBtn() {
		this.isFormHidden = true;
		this.formSpaceName = "";
		this.form.reset();
	}

	displayUserLogin(value) {
		return value?.login;
	}

	onSubmit(): void {
		if (this.form.invalid) {
			return;
		}
		if (!this.formUser) {
			return;
		}
		this.space.createInvitation(
			this.formSpaceId,
			this.formUser.id,
			this.form.value.role
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

	private fillType() {
		const itemType = item => {
			if (item.userId == this.currentUserId) {
				if (item.creatorId == item.userId)
					return INVITATION_TYPE.MY_REQUEST;
				else
					return INVITATION_TYPE.I_WAS_INVITED;
			} else {
				if (item.creatorId == item.userId)
					return INVITATION_TYPE.WANTS_TO_JOIN;
				else
					return INVITATION_TYPE.WE_INVITED;
			}
			return null;
		};

		for (const item of this.items) {
			item.type = itemType(item);
		}
	}

	changeRole(value, item: Detail) {
		if (item.type == INVITATION_TYPE.WE_INVITED || item.type == INVITATION_TYPE.WANTS_TO_JOIN) {
			this.space.changeRoleInInvitation(item.id, value)
				.subscribe(res => {
					if (res) {
						if (this.paginator.pageIndex == 0) {
							this.getItems(this.pageSize, 0);
						} else {
							this.paginator.firstPage();
						}
					}
				})
		}
	}

	approveInvitation(item: Detail) {
		if (!item.role) {
			return;
		}
		this.space.approveInvitation(item.id)
			.subscribe(_ => {
				if (this.paginator.pageIndex == 0) {
					this.getItems(this.pageSize, 0);
				} else {
					this.paginator.firstPage();
				}
			});
	}
}
