import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import {switchMap, delay, tap} from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceInvitation, SpaceLink, SpaceListResponse, SpaceUser, SpaceFields, SpaceServiceInfo} from './model';
import { PaginatorApi } from '../user';
import { RequestWatcherService } from '../request-watcher/service';

@Injectable()
export class SpaceService {
	private _isChecked = false;
	private _isInitialized: ReplaySubject<SpaceServiceInfo> = new ReplaySubject(1);
	private _current: Space = null;

	private _apiUrl = '/api/space';

	spaces: Space[] = [
		{id: "11111111-1111-1111-1111-111111111111", name: "Пространство №1", key: "key1", requestsAllowed: false, createdAt: new Date("2023-10-01")},
		{id: "22222222-2222-2222-2222-222222222222", name: "Пространство №2", key: "key2", requestsAllowed: true, createdAt: new Date("2023-10-02")},
		{id: "33333333-3333-3333-3333-333333333333", name: "Пространство №3", key: "key3", requestsAllowed: true, createdAt: new Date("2023-10-03")},
		{id: "44444444-4444-4444-4444-444444444444", name: "Пространство №4", key: "key4", requestsAllowed: true, createdAt: new Date("2023-10-04")},
		{id: "55555555-5555-5555-5555-555555555555", name: "Пространство №5", key: "key5", requestsAllowed: true, createdAt: new Date("2023-10-05")},
		{id: "66666666-6666-6666-6666-666666666666", name: "Пространство №6", key: "key6", requestsAllowed: true, createdAt: new Date("2023-10-06")},
		{id: "77777777-7777-7777-7777-777777777777", name: "Пространство №7", key: "key7", requestsAllowed: true, createdAt: new Date("2023-10-07")},
		{id: "88888888-8888-8888-8888-888888888888", name: "Пространство №8", key: "key8", requestsAllowed: true, createdAt: new Date("2023-10-08")},
		{id: "99999999-9999-9999-9999-999999999999", name: "Пространство №9", key: "key9", requestsAllowed: true, createdAt: new Date("2023-10-09")},
		{id: "10000000-1000-1000-1000-100000000000", name: "Пространство №10", key: "key10", requestsAllowed: true, createdAt: new Date("2023-10-09")},
		{id: "11000000-1100-1100-1100-110000000000", name: "Пространство №11", key: "key11", requestsAllowed: true, createdAt: new Date("2023-10-09")},
	];

	invitations: SpaceInvitation[] = [
		// Меня пригласили
		{id: Math.random(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", role: "user", creatorId: "1", createdAt: new Date("2023-10-01")},
		{id: Math.random(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", role: "user", creatorId: "1", createdAt: new Date("2023-10-02")},
		// Я прошусь
		{id: Math.random(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", role: "", creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", createdAt: new Date("2023-10-03")},
		{id: Math.random(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", role: "", creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", createdAt: new Date("2023-10-04")},
		// Мы пригласили
		{id: Math.random(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "3", role: "user", creatorId: "4", createdAt: new Date("2023-10-05")},
		{id: Math.random(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "1", role: "guest", creatorId: "4", createdAt: new Date("2023-10-06")},
		// Хочет к нам
		{id: Math.random(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-07")},
		{id: Math.random(), spaceId: '88888888-8888-8888-8888-888888888888', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-08")},
		{id: Math.random(), spaceId: '99999999-9999-9999-9999-999999999999', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-09")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-10")},
		{id: Math.random(), spaceId: '22222222-2222-2222-2222-222222222222', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-11")},
		{id: Math.random(), spaceId: '33333333-3333-3333-3333-333333333333', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-12")},
		{id: Math.random(), spaceId: '44444444-4444-4444-4444-444444444444', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-13")},
		{id: Math.random(), spaceId: '55555555-5555-5555-5555-555555555555', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-14")},
		{id: Math.random(), spaceId: '66666666-6666-6666-6666-666666666666', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-15")},
		{id: Math.random(), spaceId: '77777777-7777-7777-7777-777777777777', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-16")},
		// Далее данные для Space Detail Page
		// Мы пригласили
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "4", createdAt: new Date("2023-10-05")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "1", role: "guest", creatorId: "4", createdAt: new Date("2023-10-06")},
		// Хочет к нам
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-07")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-08")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-09")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-10")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-11")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-12")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-13")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-14")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "3", role: "user", creatorId: "3", createdAt: new Date("2023-10-15")},
		{id: Math.random(), spaceId: '11111111-1111-1111-1111-111111111111', userId: "15", role: "guest", creatorId: "15", createdAt: new Date("2023-10-16")},
	];

	links: SpaceLink[] = [
		{id: '11111111-1111-1111-1111-111111111111', spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link1", createdAt: new Date("2023-10-08"), expiredAt: new Date("2023-12-08")},
		{id: crypto.randomUUID(), spaceId: '22222222-2222-2222-2222-222222222222', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '33333333-3333-3333-3333-333333333333', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '44444444-4444-4444-4444-444444444444', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '55555555-5555-5555-5555-555555555555', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '66666666-6666-6666-6666-666666666666', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '77777777-7777-7777-7777-777777777777', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '88888888-8888-8888-8888-888888888888', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link11", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link2", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link3", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link4", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link5", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link6", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link7", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link8", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
	];

	users: SpaceUser[] = [
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "1", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", isOwner: true, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "3", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "4", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "5", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "6", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "7", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "8", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "9", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "10", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "11", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "12", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "13", isOwner: false, joinedAt: new Date("2024-10-05"), role: "admin"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "14", isOwner: false, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "11111111-1111-1111-1111-111111111111", userId: "15", isOwner: false, joinedAt: new Date("2024-10-05"), role: "guest"},
		{spaceId: "22222222-2222-2222-2222-222222222222", userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", isOwner: true, joinedAt: new Date("2024-10-05"), role: "user"},
		{spaceId: "33333333-3333-3333-3333-333333333333", userId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", isOwner: true, joinedAt: new Date("2024-10-05"), role: "user"}
	];

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
		// TODO: remove next line
		return of({} as SpaceServiceInfo).pipe(
		// TODO: uncomment next line
		// return this.http.get<SpaceServiceInfo>(this._apiUrl + '/info').pipe(
			tap(res => this._isInitialized.next(res))
		);
	}

	isInitialized(): Observable<SpaceServiceInfo> {
		return this._isInitialized.asObservable();
	}

	getList(limit: number, page: number, name: string = ''): Observable<PaginatorApi<Space>> {
		const grouped = this.spaces.filter(space => space.name.includes(name));
		const res: PaginatorApi<Space> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getAvailableList(limit: number, page: number, name: string = '', userId: string): Observable<PaginatorApi<Space>> {
		const grouped = this.spaces.filter(space => {
			return space.requestsAllowed
				&& space.name.includes(name)
				&& !this.users.find(u => u.spaceId == space.id && u.userId == userId);
		});

		const res: PaginatorApi<Space> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getById(spaceId: string) {
		let space = this.spaces.find(s => s.id === spaceId);
		return of(space)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			);
	}

	getByKey(spaceKey: string) {
		let space = this.spaces.find(s => s.key === spaceKey);
		return of(space)
			.pipe(delay(2000));
	}

	getByLink(linkId: string): Observable<Space> {
		let link = this.links.find(l => l.id === linkId);
		if (link) {
			let space = this.spaces.find(s => s.id === link.spaceId);
			if (space) {
				return of(space)
					.pipe(
						delay(2000),
						src => this.requestWatcher.WatchFor(src)
					);
			}
		}
	}

	getInvitationList(limit: number, page: number, spaceId: string = null): Observable<PaginatorApi<SpaceInvitation>> {
		const grouped = spaceId ?
			this.invitations.filter(item => item.spaceId === spaceId) :
			this.invitations;

		const res: PaginatorApi<SpaceInvitation> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getUserList(spaceId: string, limit: number, page: number): Observable<PaginatorApi<SpaceUser>> {
		let grouped: SpaceUser[] = [];
		this.users.forEach(function(user) {
			if (user.spaceId === spaceId) {
				grouped.push(user);
			}
		});
		const res: PaginatorApi<SpaceUser> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getLinkList(limit: number, page: number, spaceId: string = null): Observable<PaginatorApi<SpaceLink>> {
		const grouped = spaceId ?
			this.links.filter(item => item.spaceId === spaceId) :
			this.links;
		const res: PaginatorApi<SpaceLink> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	isExists(key: string): Observable<boolean> {
		return of(this.spaces.some(s => s.key === key))
			.pipe(delay(2000));
	}

	createNew(name: string, key: string, requestsAllowed: boolean): Observable<boolean> {
		this.spaces.push({
			id: crypto.randomUUID(),
			name: name,
			key: key,
			requestsAllowed: requestsAllowed,
			createdAt: new Date()
		});
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	createInvitation(spaceId: string, userId: string, role: string, creatorId: string): Observable<boolean> {
		this.invitations.push({
			id: Math.random(),
			spaceId: spaceId,
			userId: userId,
			role: role,
			creatorId: creatorId,
			createdAt: new Date(),
		});
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	createLink(spaceId: string, creatorId: string, name: string, expiredAt: Date): Observable<boolean> {
		this.links.push({
			id: crypto.randomUUID(),
			spaceId: spaceId,
			creatorId: creatorId,
			name: name,
			createdAt: new Date(),
			expiredAt: expiredAt
		});
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	delInvitationById(invitationId: number): Observable<boolean> {
		return of(true).pipe(
			delay(2000),
			switchMap(val => {
				const index = this.invitations.findIndex(x => x.id === invitationId);
				if (index < 0)
				{
					const err = new Error("Invalid Id");
					return throwError(err);
				}
				this.invitations.splice(index, 1);
				return of(val);
			}),
			src => this.requestWatcher.WatchFor(src)
		);
	}

	delLinkById(linkId: string): Observable<boolean> {
		return of(true).pipe(
			delay(2000),
			switchMap(val => {
				const index = this.links.findIndex(x => x.id === linkId);
				if (index < 0)
				{
					const err = new Error("Invalid Id");
					return throwError(err);
				}
				this.links.splice(index, 1);
				return of(val);
			}),
			src => this.requestWatcher.WatchFor(src)
		);
	}

	delById(spaceId: string): Observable<boolean> {
		return of(true).pipe(
			delay(2000),
			switchMap(val => {
				const index = this.spaces.findIndex(x => x.id === spaceId);
				if (index < 0)
				{
					const err = new Error("Invalid Id");
					return throwError(err);
				}
				this.spaces.splice(index, 1);
				return of(val);
			}),
			src => this.requestWatcher.WatchFor(src)
		);
	}

	delUserById(userId: string): Observable<boolean> {
		return of(true).pipe(
			delay(2000),
			switchMap(val => {
				const index = this.users.findIndex(x => x.userId === userId);
				if (index < 0)
				{
					const err = new Error("Invalid Id");
					return throwError(err);
				}
				this.users.splice(index, 1);
				return of(val);
			}),
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

	join(spaceId: string): Observable<boolean> {
		let space = this.spaces.find(s => s.id === spaceId);
		if (space) {
			return of(space.requestsAllowed)
				.pipe(
					delay(2000),
					src => this.requestWatcher.WatchFor(src)
				)
		} else {
			return of(false)
				.pipe(
					delay(2000),
					src => this.requestWatcher.WatchFor(src)
				)
		}
	}

	joinByLink(token: string): Observable<boolean> {
		let link = this.links.find(l => l.id === token);
		let now = new Date();
		if (link.expiredAt > now) {
			let space = this.spaces.find(s => s.id === link.spaceId);
			return of(true)
				.pipe(
					delay(2000),
					src => this.requestWatcher.WatchFor(src)
				)
		}
	}

	changeRoleInInvitation(id, newRole): Observable<boolean> {
		let index = this.invitations.findIndex(i => i.id === id);
		if (index > -1) {
			this.invitations[index].role = newRole;
			return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
		}
		return of(false)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	approveInvitation(id: number): Observable<boolean> {
		let index = this.invitations.findIndex(i => i.id === id);

		if (index > -1) {
			let invitation = this.invitations[index];

			this.users.push({spaceId: invitation.spaceId, userId: invitation.userId, isOwner: false, joinedAt: new Date(), role: invitation.role})

			this.invitations.splice(index, 1);

			return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
		}
		return of(false)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}
}