import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Observable, of } from 'rxjs';
import { catchError, map } from 'rxjs/operators';

import { AuthService } from '../../auth/service';
import { SpaceService } from '../service';
import { SpaceVisitHolder } from '../visit-holder';
import { Space, Spaces } from '../../api';

@Component({
	selector: 'app-space-auto-select',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceAutoSelectComponent implements OnInit {
	constructor(
		private auth: AuthService,
		private space: SpaceService,
		private spaceVisitHolder: SpaceVisitHolder,
		private router: Router,
	) { }

	ngOnInit() {
		// проверяем заходил ли пользователь раньше в какое то пространство
		const spacePath = this.spaceVisitHolder.get();
		if (!spacePath)
			// если нет, то проверяем список пространств
			this.checkSpaceList();
		else
			// если заходил раньше, то проверяем доступно ли оно сейчас
			this.checkPathExists(spacePath).subscribe(ok => {
				if (ok)
					// если всё ещё доступно, переходим в него
					this.router.navigate([`/${spacePath}/dashboard`]);
				else
					this.checkSpaceList();
			});
	}

	// проверка валидности пути к пространству
	checkPathExists(spacePath: string): Observable<boolean> {
		if (spacePath == 'u')
			return of(true);

		const re: RegExp = /^s\/([^\/]+)$/
		const match = re.exec(spacePath);
		if (!match)
			return of(false);

		return this.space.getByKey(match[1]).pipe(
			map(() => true),
			catchError(() => of(false)),
		);
	}

	// запрос для проверки списка пространств
	checkSpaceList() {
		this.space.getList(1, 0).subscribe(res => this.chooseFromList(res));
	}

	// проверка списка пространств
	chooseFromList(spaces: Spaces) {
		if (spaces.total == 1)
			this.useSingle(spaces.list[0]);
		else if (spaces.total > 1)
			this.router.navigate([`/space/list`]);
		else
			this.checkInvitations();
	}

	// переход на роут единственного пространства
	useSingle(space: Space) {
		let spacePath = `s/${space.key}`;
		if (space.key == this.auth.user.id)
			spacePath = 'u';

		this.spaceVisitHolder.set(spacePath);

		// TODO: передавать параметрами инфу о space чтобы не делать внутри повторный запрос
		this.router.navigate([`/${spacePath}/dashboard`]);
	}

	// Проверяет есть ли приглашения в пространства
	checkInvitations() {
		this.space.getInvitationList(1, 0).subscribe(
			// если есть, то отправляем на роут списка приглашений
			() => this.router.navigate([`/space/list`]),

			// если нет, то отправляем на роут создания проекта
			() => this.router.navigate([`/u/project/create`]),
		);
	}
}
