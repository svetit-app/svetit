import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {tap, catchError, map} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceFields, SpaceServiceInfo} from './model';
import { RequestWatcherService } from '../request-watcher/service';
import { SpaceService as ApiSpaceService, Invitations, Links, Users, InvitationRole, SpaceParams, Spaces} from '../api';

@Injectable()
export class SpaceService {
	private _isChecked = false;
	private _isInitialized: ReplaySubject<SpaceServiceInfo> = new ReplaySubject(1);
	private _current: Space = null;

	private _apiUrl = '/api/space';

	get current() {
		return this._current;
	}

	constructor(
		private http: HttpClient,
		private requestWatcher: RequestWatcherService,
		private api: ApiSpaceService,
	) {
	}

	Check(): Observable<SpaceParams> {
		if (this._isChecked)
			return this.isInitialized();

		this._isChecked = true;

		return this.api.infoGet().pipe(
			tap(res => this._isInitialized.next(res)),
			src => this.requestWatcher.WatchFor(src),
		);
	}

	isInitialized(): Observable<SpaceServiceInfo> {
		return this._isInitialized.asObservable();
	}

	getList(limit: number, page: number, name: string = ''): Observable<Spaces> {
		return this.api.listGet(limit*page, limit).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getAvailableList(limit: number, page: number, name: string = undefined): Observable<Spaces> {
		return this.api.listAvailableGet(limit*page, limit, name).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getById(spaceId: string): Observable<any> {
		return this.api.spaceGet(spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByKey(spaceKey: string): Observable<any> {
		return this.api.spaceGet(undefined, spaceKey).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByLink(linkId: string): Observable<any> {
		return this.api.spaceGet(undefined, undefined, linkId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getInvitationList(limit: number, page: number, spaceId: string = undefined): Observable<Invitations> {
		return this.api.invitationGet(limit*page, limit, spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getUserList(spaceId: string, limit: number, page: number): Observable<Users> {
		return this.api.spaceUserListGet(spaceId, limit*page, limit).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getLinkList(limit: number, page: number, spaceId: string = undefined): Observable<Links> {
		return this.api.linkGet(limit*page, limit, spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	isExists(key: string): Observable<boolean> {
		return this.api.spaceHead(key)
			.pipe(
				src => this.requestWatcher.WatchFor(src),
				map(_ => true),
				catchError(err => {
					if (err.status == 404)
						return of(false);
					return throwError(() => err);
				}),
			);
	}

	createNew(name: string, key: string, requestsAllowed: boolean): Observable<any> {
		return this.api.spacePost({
			name: name,
			key: key,
			requestsAllowed: requestsAllowed
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createInvitation(spaceId: string, userId: string, role: string): Observable<any> {
		return this.api.invitationPost({
			spaceId: spaceId,
			userId: userId,
			roleId: parseInt(role)
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createLink(spaceId: string, name: string, expiredAt: Date): Observable<any> {
		return this.api.linkPut({
			spaceId: spaceId,
			name: name,
			expiredAt: (new Date(expiredAt).getTime()/1000)
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	delInvitationById(invitationId: number): Observable<any> {
		return this.api.invitationDelete(invitationId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delLinkById(linkId: string): Observable<any> {
		return this.api.linkDelete(linkId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delById(spaceId: string): Observable<any> {
		return this.api.spaceDelete(spaceId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delUserById(userId: string, spaceId: string): Observable<any> {
		return this.api.userManageDelete(userId, spaceId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	fillFields<T extends SpaceFields & {spaceId: string}>(items: T[]) {
		const ids = new Set<string>();
		items.forEach(item => ids.add(item.spaceId));
		for (const id of ids) {
			this.getById(id).subscribe(space => {
				for (const item of items) {
					if (item.spaceId !== space.id)
						continue;
					item.spaceName = space.name;
				}
			});
		}
	}

	join(spaceId: string, userId: string): Observable<any> {
		return this.api.invitationPost({
			spaceId: spaceId,
			creatorId: userId,
			userId: userId,
			roleId: 1 // hardcoded operator roleId (1)
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	joinByLink(token: string): Observable<any> {
		return this.api.linkPost(token)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	changeRoleInInvitation(id: number, role: string): Observable<any> {
		let roleId = parseInt(role);
		return this.api.invitationPut(id, {roleId: roleId}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	approveInvitation(id: number): Observable<any> {
		return this.api.invitationPatch(id).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	resetIsChecked() {
		this._isChecked = false;
	}
}
