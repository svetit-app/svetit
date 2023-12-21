import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {tap, catchError, map} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceInvitation, SpaceLink, SpaceUser, SpaceFields, SpaceServiceInfo} from './model';
import { Paging } from '../user';
import { RequestWatcherService } from '../request-watcher/service';
import { SpaceRole } from './model'

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
	) {
	}

	Check(): Observable<SpaceServiceInfo> {
		if (this._isChecked)
			return this.isInitialized();

		this._isChecked = true;
		return this.http.get<SpaceServiceInfo>(this._apiUrl + '/info').pipe(
			tap(res => this._isInitialized.next(res)),
			src => this.requestWatcher.WatchFor(src),
		);
	}

	isInitialized(): Observable<SpaceServiceInfo> {
		return this._isInitialized.asObservable();
	}

	getList(limit: number, page: number, name: string = ''): Observable<Paging<Space>> {
		return this.http.get<Paging<Space>>(this._apiUrl + "/list?start=" + limit*page + "&limit=" + limit).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getAvailableList(limit: number, page: number, name: string = ''): Observable<Paging<Space>> {
		let url = this._apiUrl + "/available/list?start=" + limit*page + "&limit=" + limit;
		if (name)
			url += '&spaceName=' + name;
		return this.http.get<Paging<Space>>(url).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getById(spaceId: string): Observable<any> {
		return this.http.get<Space>(this._apiUrl + "?id=" + spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByKey(spaceKey: string): Observable<any> {
		return this.http.get<Space>(this._apiUrl + "?key=" + spaceKey).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getByLink(linkId: string): Observable<any> {
		return this.http.get<Space>(this._apiUrl + "?link=" + linkId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getInvitationList(limit: number, page: number, spaceId: string = null): Observable<Paging<SpaceInvitation>> {
		let url = this._apiUrl + "/invitation?start=" + limit*page + "&limit=" + limit;
		if (spaceId)
			url += '&spaceId=' + spaceId;
		return this.http.get<Paging<SpaceInvitation>>(url).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getUserList(spaceId: string, limit: number, page: number): Observable<Paging<SpaceUser>> {
		return this.http.get<Paging<SpaceUser>>(this._apiUrl + "/user/list?start=" + limit*page + "&limit=" + limit + "&spaceId=" + spaceId).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	getLinkList(limit: number, page: number, spaceId: string = null): Observable<Paging<SpaceLink>> {
		let url = this._apiUrl + '/invitation/link?start=' + limit*page + '&limit=' + limit;
		if (spaceId)
			url += '&spaceId=' + spaceId;
		return this.http.get<Paging<SpaceLink>>(url).pipe(
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
			id: "",
			name: name,
			key: key,
			requestsAllowed: requestsAllowed,
			createdAt: 0
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createInvitation(spaceId: string, userId: string, role: string): Observable<any> {
		return this.http.post(this._apiUrl + "/invitation", {
			id: 0,
			spaceId: spaceId,
			creatorId: "",
			userId: userId,
			role: role,
			createdAt: 0
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	createLink(spaceId: string, name: string, expiredAt: Date): Observable<any> {
		return this.http.post(this._apiUrl + "/invitation/link", {
			id: "",
			spaceId: spaceId,
			creatorId: "",
			name: name,
			createdAt: 0,
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
			id: 0,
			spaceId: spaceId,
			creatorId: userId,
			userId: userId,
			role: SpaceRole.Guest,
			createdAt: 0,
		}).pipe(
			src => this.requestWatcher.WatchFor(src)
		);
	}

	joinByLink(token: string): Observable<any> {
		return this.http.post(this._apiUrl + "/invitation?link=" + token, {})
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
}
