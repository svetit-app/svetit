import { Injectable, inject } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { Observable } from 'rxjs/Observable';
import { of } from 'rxjs/observable/of';

import {Auth_Group, Scheme, Scheme_Group, PaginatorApi, Group_User_Roles, UserHeader, UserHeaderWithRole} from '../user';
import { MessageService } from '../message.service';
import { ISchemeService } from '../ischeme.service';
import {BehaviorSubject} from 'rxjs';
import {catchError, tap} from 'rxjs/operators';

export interface Titled_Object
{
    id: number;
    title: string;
}

export interface SearchResult {
  count: number;
  next: string;
  previous: string;
  results: Scheme[];
}

export const nullSearchResult: SearchResult = {
  count: 0,
  next: null,
  previous: null,
  results: []
};

@Injectable()
export class SchemesService extends ISchemeService {

    schemeGroupsSubject: BehaviorSubject<Scheme_Group[]> = new BehaviorSubject([]);

  constructor() {
    super();

    this.get_scheme_groups_();
  }

    private v2_url = '/api/v2/';
    private v2_scheme_url = this.v2_url + 'scheme/';

    private schemeUrl = 'scheme/';  // URL to web api
    private cityUrl = 'city/';
    private compUrl = 'company/';

    getSchemes(limit: number, page: number = 0, ordering?: string, query?: string): Observable<PaginatorApi<Scheme>> {
        let url = this.schemeUrl + `?limit=${limit}&offset=${limit * page}`;
        if (ordering && ordering.length) {
            url += '&ordering=' + ordering;
        }

        if (query && query.length) {
            url += '&search=' + query;
        }

        return this.getPiped<PaginatorApi<Scheme>>(url,
            `fetched client devices`, 'getSchemes', {} as PaginatorApi<Scheme>);
    }

    getUserHeaders(limit: number, page: number = 0): Observable<UserHeader[]> {
        return this.http.get<UserHeader[]>(this.v2_url + `user/?limit=${limit}&offset=${limit * page}&header=1`);
    }

    getSchemeGroupUsers(id: number): Observable<UserHeaderWithRole[]> {
        const url = this.v2_url + `scheme_group/${id}/user/`;
        return this.http.get<UserHeaderWithRole[]>(url);
    }

    getSchemeGroupSchemes(id: number): Observable<Pick<Scheme, 'id' | 'name' | 'title'>[]> {
        const url = this.v2_url + `scheme_group/${id}/scheme/`;
        return this.http.get<Pick<Scheme, 'id' | 'name' | 'title'>[]>(url);
    }

    addSchemeToSchemeGroup(groupId: number, schemeId: number): Observable<{scheme_group_id: number, scheme_id: number}> {
        const url = this.v2_url + `scheme_group/${groupId}/scheme/${schemeId}/`;
        return this.http.post<{scheme_group_id: number, scheme_id: number}>(url, {});
    }

    removeSchemeFromSchemeGroup(groupId: number, schemeId: number): Observable<null> {
        const url = this.v2_url + `scheme_group/${groupId}/scheme/${schemeId}/`;
        return this.http.delete<null>(url);
    }

    addUserToSchemeGroup(groupId: number, userId: string, role: any): Observable<{group_id: number, user_id: string, role: string}> {
        const url = this.v2_url + `scheme_group/${groupId}/user/${userId}/?role=${role}`;
        return this.http.post<{group_id: number, user_id: string, role: string}>(url, {});
    }

    removeUserFromSchemeGroup(groupId: number, userId: string): Observable<null> {
        const url = this.v2_url + `scheme_group/${groupId}/user/${userId}/`;
        return this.http.delete<null>(url);
    }

    createSchemeGroup(value: Scheme_Group): Observable<Scheme_Group> {
        const url = this.v2_url + `scheme_group/`;
        return this.http.post<Scheme_Group>(url, value)
            .pipe(tap(() => this.get_scheme_groups_()));
    }

    updateSchemeGroup(value: Scheme_Group): Observable<Scheme_Group> {
        const url = this.v2_url + `scheme_group/`;
        return this.http.put<Scheme_Group>(url, value)
            .pipe(tap(() => this.get_scheme_groups_()));
    }

    removeSchemeGroup(id: number): Observable<null> {
        const url = this.v2_url + `scheme_group/${id}/`;
        return this.http.delete<null>(url)
            .pipe(tap(() => this.get_scheme_groups_()));
    }

    getSchemeGroup(id: number): Observable<Scheme_Group> {
        const url = this.v2_url + `scheme_group/${id}/`;
        return this.http.get<Scheme_Group>(url);
    }

    getSchemeGroupsForScheme(schemeId: number): Observable<Scheme_Group[]> {
        const url = this.v2_url + `scheme_group/?scheme_id=${schemeId}`;
        return this.http.get<Scheme_Group[]>(url);
    }

    get_scheme_groups(): Observable<Scheme_Group[]>
    {
        return this.schemeGroupsSubject.asObservable();
    }

    getSchemeGroupsForUser(userId: string): Observable<Scheme_Group[]> {
        const url = this.v2_url + `scheme_group/?user_id=${userId}`;
        return this.http.get<Scheme_Group[]>(url);
    }

    private get_scheme_groups_()
    {
        const url = this.v2_url + 'scheme_group/';
        this.http.get<Scheme_Group[]>(url)
            .subscribe(v => this.schemeGroupsSubject.next(v));
    }

    get_parent_schemes(exclude_id: number = undefined): Observable<Titled_Object[]>
    {
        let url = this.v2_scheme_url + '?parent_id=null';
        if (exclude_id)
            url += `&id!=${exclude_id}`;
        return this.http.get<Titled_Object[]>(url);
    }

    getCities(): Observable<PaginatorApi<any>> {
        const url = this.cityUrl;

        return this.getPiped<PaginatorApi<any>>(url,
            `fetched cities`, 'getCities', {} as PaginatorApi<Scheme>);
    }

    getCompanies(): Observable<PaginatorApi<any>> {
        const url = this.compUrl;

        return this.getPiped<PaginatorApi<any>>(url,
            `fetched cities`, 'getCities', {} as PaginatorApi<Scheme>);
    }

    getAuthGroups(): Observable<Auth_Group[]>
    {
        const url = '/api/v2/auth_group';
		return this.http.get<Auth_Group[]>(url).pipe(
			catchError((err: HttpErrorResponse) => of([]))
		);
    }

    setName(schemeId: number, name: string): Observable<any> {
        const url = `/api/v2/scheme/${schemeId}/set_name/`;
        return this.http.post<any>(url, {name});
    }

  getScheme(name: string): Observable<Scheme> {
    const url = `${this.schemeUrl}${name}/`;
    return this.getPiped<Scheme>(url, `fetched client device name=${name}`, `getScheme name=${name}`);
  }

  /** PUT: update the scheme on the server */
  updateScheme(scheme: Scheme): Observable<any> {
    return this.putPiped(`${this.schemeUrl}${scheme.name}/`, scheme, `updated client device id=${scheme.id}`, 'updateScheme');
  }

    /** POST: add a new scheme to the server */
    create_scheme(scheme: any): Observable<Scheme> {
        const url = this.v2_scheme_url;
		return this.http.post<Scheme>(url, scheme).pipe(
			catchError((err: HttpErrorResponse) => {
				alert(err.error + '\n' + err.message);
				return of(null as Scheme);
			})
		);
    }

  /** DELETE: delete the scheme from the server */
  deleteScheme(scheme: Scheme | number): Observable<Scheme> {
    const id = typeof scheme === 'number' ? scheme : scheme.id;
    const url = `${this.schemeUrl}${id}/`;

    return this.deletePiped<Scheme>(url, `deleted client device id=${id}`, 'deleteScheme');
  }

  /* GET schemes whose name contains search term */
  searchSchemes(term: string, next?: string): Observable<SearchResult> {
    if (!term.trim()) {
      // if not search term, return empty scheme array.
      return of(nullSearchResult);
    }
    const url = this.schemeUrl + (next ? next : `?search=${term}`);

    return this.getPiped<SearchResult>(url, `found schemes matching "${term}"`, 'searchSchemes', nullSearchResult);
  }
}
