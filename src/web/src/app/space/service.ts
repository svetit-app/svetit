import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {tap, catchError, map} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceInvitation, SpaceLink, SpaceUser, SpaceFields, SpaceServiceInfo} from './model';
import { Paging } from '../user';
import { RequestWatcherService } from '../request-watcher/service';
import { SpaceRole } from './model'
import { SpaceService as ApiSpaceService, Invitations as ApiInvitations, Links as ApiLinks, Users as ApiUsers} from '../api';
import { SpaceParams as ApiSpaceParams} from '../api';
import { Spaces as ApiSpaces } from '../api';

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
		private apiSpaceService: ApiSpaceService,
	) {
	}

	Check(): Observable<ApiSpaceParams> {
		if (this._isChecked)
			return this.isInitialized();

		this._isChecked = true;

		return this.apiSpaceService.handlerInfoGet("user").pipe(
			tap(res => this._isInitialized.next(res)),
			src => this.requestWatcher.WatchFor(src),
		);
	}

	isInitialized(): Observable<SpaceServiceInfo> {
		return this._isInitialized.asObservable();
	}

	getList(limit: number, page: number, name: string = ''): Observable<ApiSpaces> {
		return this.apiSpaceService.handlerListGet("user", limit*page, limit).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getAvailableList(limit: number, page: number, name: string = ''): Observable<ApiSpaces> {
		if (name)
			return this.apiSpaceService.handlerListAvailableGet("user", limit*page, limit, name).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
		else
			return this.apiSpaceService.handlerListAvailableGet("user", limit*page, limit).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	getById(spaceId: string): Observable<any> {
		return this.apiSpaceService.handlerSpaceGet("user", spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByKey(spaceKey: string): Observable<any> {
		return this.apiSpaceService.handlerSpaceGet("user", undefined, spaceKey).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByLink(linkId: string): Observable<any> {
		return this.apiSpaceService.handlerSpaceGet("user", undefined, undefined, linkId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getInvitationList(limit: number, page: number, spaceId: string = null): Observable<ApiInvitations> {
		if (spaceId)
			return this.apiSpaceService.handlerInvitationGet("user", limit*page, limit, spaceId).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
		else
			return this.apiSpaceService.handlerInvitationGet("user", limit*page, limit).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	getUserList(spaceId: string, limit: number, page: number): Observable<ApiUsers> {
		return this.apiSpaceService.handlerSpaceUserListGet("user", spaceId, limit*page, limit).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getLinkList(limit: number, page: number, spaceId: string = null): Observable<ApiLinks> {
		if (spaceId)
			return this.apiSpaceService.handlerLinkGet("user", limit*page, limit, spaceId).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
		else
			return this.apiSpaceService.handlerLinkGet("user", limit*page, limit).pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	isExists(key: string): Observable<boolean> {
		return this.http.head(this._apiUrl + "/?key=" + key)
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
		return this.http.post(this._apiUrl, {
			name: name,
			key: key,
			requestsAllowed: requestsAllowed
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createInvitation(spaceId: string, userId: string, role: string): Observable<any> {
		return this.http.post(this._apiUrl + "/invitation", {
			spaceId: spaceId,
			userId: userId,
			role: role
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createLink(spaceId: string, name: string, expiredAt: Date): Observable<any> {
		return this.http.put(this._apiUrl + "/invitation/link", {
			spaceId: spaceId,
			name: name,
			expiredAt: (new Date(expiredAt).getTime()/1000)
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	delInvitationById(invitationId: number): Observable<any> {
		return this.http.delete(this._apiUrl + "/invitation?id=" + invitationId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delLinkById(linkId: string): Observable<any> {
		return this.http.delete(this._apiUrl + "/invitation/link?id=" + linkId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delById(spaceId: string): Observable<any> {
		return this.http.delete(this._apiUrl + "?id=" + spaceId)
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	delUserById(userId: string, spaceId: string): Observable<any> {
		return this.http.delete(this._apiUrl + "/user?spaceId=" + spaceId + "&userId=" + userId)
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
		return this.http.post(this._apiUrl + "/invitation", {
			spaceId: spaceId,
			creatorId: userId,
			userId: userId,
			role: SpaceRole.Guest
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	joinByLink(token: string): Observable<any> {
		return this.http.post(this._apiUrl + "/invitation/link?id=" + token, {})
			.pipe(
				src => this.requestWatcher.WatchFor(src)
			);
	}

	changeRoleInInvitation(id: number, role: string): Observable<any> {
		return this.http.put(this._apiUrl + "/invitation?id=" + id, { role }).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	approveInvitation(id: number): Observable<any> {
		return this.http.patch(this._apiUrl + "/invitation?id=" + id, {}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	resetIsChecked() {
		this._isChecked = false;
	}
}
