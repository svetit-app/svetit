import {Injectable, Injector} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {ReplaySubject, of, throwError} from 'rxjs';
import { catchError, switchMap, delay } from 'rxjs/operators';
import {Observable} from 'rxjs/Observable';

import { Space, SpaceInvitation, SpaceLink, SpaceListResponse, SpaceUser, SpaceInvitationFields, SpaceLinkFields} from './model';
import { NavigationExtras, Router } from '@angular/router';
import { PaginatorApi } from '../user';
import { RequestWatcherService } from '../request-watcher/service';
import { UserService } from '../user/service';

type SpaceInvitationDetail = SpaceInvitation & SpaceInvitationFields;
type SpaceLinkDetail = SpaceLink & SpaceLinkFields;

@Injectable()
export class SpaceService {
	private _isChecked = false;
	private _isInitialized: ReplaySubject<boolean> = new ReplaySubject();
	private _current: Space = null;
	private _items: Space[] = [];

	private _apiUrl = '/api/space/';

	spaces: Space[] = [
		{id: "11111111-1111-1111-1111-111111111111", name: "Пространство №1", key: "key1", requestsAllowed: true, createdAt: new Date("2023-10-01")},
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
		{id: crypto.randomUUID(), spaceId: '11111111-1111-1111-1111-111111111111', creatorId: "8ad16a1d-18b1-4aaa-8b0f-f61915974c66", name: "link1", createdAt: new Date("2023-10-08"), expiredAt: new Date("2024-10-08")},
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
	];

	get current() {
		return this._current;
	}

	get items() {
		return this._items;
	}

	constructor(
		private http: HttpClient,
		private router: Router,
		private requestWatcher: RequestWatcherService,
		private injector: Injector,
	) {
	}

	Check(): Observable<boolean> {
		if (this._isChecked)
			return this.isInitialized();
		return this.Fetch();
	}

	Fetch(): Observable<boolean> {
		this._isChecked = true;
		return this.http.get<SpaceListResponse>(this._apiUrl + 'list').pipe(
			switchMap(res => {
				this._items = res.items;
				this._isInitialized.next(true);
				return of(true);
			})
		);
	}

	isInitialized(): Observable<boolean> {
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

	getById(spaceId: string) {
		let space = this.spaces.find(s => s.id === spaceId);
		return of(space)
			.pipe(delay(2000));
	}

	getInvitationList(limit: number, page: number): Observable<PaginatorApi<SpaceInvitation>> {
		const res: PaginatorApi<SpaceInvitation> = {
			count: this.invitations.length,
			results: this.invitations.slice(limit * page, limit * page + limit),
		};
		return of(res).
			pipe(delay(2000));
	}

	getInvitationListForSpace(spaceId: string, limit: number, page: number): Observable<PaginatorApi<SpaceInvitation>> {
		let grouped: SpaceInvitation[] = [];
		this.invitations.forEach(function(invitation) {
			if (invitation.spaceId === spaceId){
				grouped.push(invitation);
			}
		});
		const res: PaginatorApi<SpaceInvitation> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getLinkListForSpace(spaceId: string, limit: number, page: number): Observable<PaginatorApi<SpaceLink>> {
		let grouped: SpaceLink[] = [];
		this.links.forEach(function(link) {
			if (link.spaceId === spaceId){
				grouped.push(link);
			}
		});
		const res: PaginatorApi<SpaceLink> = {
			count: grouped.length,
			results: grouped.slice(limit * page, limit * page + limit),
		};
		return of(res)
			.pipe(delay(2000));
	}

	getUserListForSpace(spaceId: string, limit: number, page: number): Observable<PaginatorApi<SpaceUser>> {
		let grouped: SpaceUser[] = [];
		this.users.forEach(function(user) {
			if (user.spaceId === spaceId){
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

	getLinkList(limit: number, page: number): Observable<PaginatorApi<SpaceLink>> {
		const res: PaginatorApi<SpaceLink> = {
			count: this.links.length,
			results: this.links.slice(limit * page, limit * page + limit),
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
		const index = this.invitations.findIndex(x => x.id === invitationId);
		if (index > -1) {
			this.invitations.splice(index, 1);
		}
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	delLinkById(linkId: string): Observable<boolean> {
		const index = this.links.findIndex(x => x.id === linkId);
		if (index > -1) {
			this.links.splice(index, 1);
		}
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	delSpaceById(spaceId: string): Observable<boolean> {
		const index = this.spaces.findIndex(x => x.id === spaceId);
		if (index > -1) {
			this.spaces.splice(index, 1);
		}
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	delUserById(userId: string): Observable<boolean> {
		const index = this.users.findIndex(x => x.userId === userId);
		if (index > -1) {
			this.users.splice(index, 1);
		}
		return of(true)
			.pipe(
				delay(2000),
				src => this.requestWatcher.WatchFor(src)
			)
	}

	fillInvitationDetailFields(invitations: SpaceInvitationDetail[]) {
		let invitationsArray: SpaceInvitationDetail[] = [];
		invitations.forEach(invite => invitationsArray.push(invite));

		for (const invitation of invitationsArray) {
			this.getById(invitation.spaceId).subscribe(space => {
				for (const invitation of invitations) {
					if (invitation.spaceId !== space.id)
						continue;
					invitation.spaceName = space.name;
				}
			});
		}

		const userService = this.injector.get(UserService);
		
		for (const invitation of invitationsArray) {
			userService.getById(invitation.userId).subscribe(user => {
				for (const invitation of invitations) {
					if (invitation.userId !== user.id)
						continue;
					invitation.userLogin = user.login;
				}
			});
		}
	}

	fillLinkDetailFields(links: SpaceLinkDetail[]) {
		let linksArray: SpaceLinkDetail[] = [];
		links.forEach(invite => linksArray.push(invite));

		for (const link of linksArray) {
			this.getById(link.spaceId).subscribe(space => {
				for (const link of links) {
					if (link.spaceId !== space.id)
						continue;
					link.spaceName = space.name;
				}
			});
		}
	}
}
