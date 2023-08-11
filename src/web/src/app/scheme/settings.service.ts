import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

import { SchemeService } from './scheme.service';
import {Section, Code_Item, Plugin_Type, Save_Timer, Translation, Value_View_Detail} from './scheme';
import { PaginatorApi } from "../user";
import {HttpClient} from '@angular/common/http';

@Injectable()
export class SettingsService {
  private readonly sct_s: string = 'section/';

  constructor(
    private hServ: SchemeService,
    private http: HttpClient,
  ) {}

  getTranslations(): Observable<Translation[]> {
      return this.http.get<Translation[]>(`/api/v2/scheme/${this.hServ.scheme.id}/structure/translation/`);
  }

  getValueViewsDetail(): Observable<Value_View_Detail[]> {
      return this.http.get<Value_View_Detail[]>(`/api/v2/scheme/${this.hServ.scheme.id}/structure/value_view/`);
  }

  getPluginTypes(): Observable<PaginatorApi<Plugin_Type>> {
    const url = this.hServ.url('plugin');
    return this.hServ.getPiped<PaginatorApi<Plugin_Type>>(url, 'fetched plugins', 'getPlugins', {} as PaginatorApi<Plugin_Type>);
  }

  getSaveTimers(): Observable<PaginatorApi<Save_Timer>> {
    return this.hServ.getPiped<PaginatorApi<Save_Timer>>(this.hServ.url('savetimer'), `fetched save timer list`, 'getSaveTimers', {} as PaginatorApi<Save_Timer>);
  }

  getCodes(): Observable<Code_Item[]> {
    return this.hServ.getPiped<Code_Item[]>(this.hServ.url('code_item'), `fetched code list`, 'getCodes', []);
  }

  getCode(code_id: number): Observable<Code_Item> {
    return this.hServ.getPiped<Code_Item>(this.hServ.url('code_item', code_id), `fetched code ${code_id}`, 'getCode', {} as Code_Item);
  }

  getSection(id: number): Observable<Section> {
    const url = this.hServ.url(this.sct_s, id);
    return this.hServ.getPiped<Section>(url, `fetched section id=${id}`, `getSection id=${id}`);
  }

  /** PUT: update the scheme on the server */
  updateSection (sct: Section): Observable<any> {
    const url = this.hServ.url(this.sct_s, sct.id);
    return this.hServ.putPiped(url, sct, `updated section id=${sct.id}`, 'updateSection');
  }

  /** POST: add a new scheme to the server */
  addSection (sct: Section): Observable<Section> {
    const url = this.hServ.url(this.sct_s);
    return this.hServ.postPiped<Section>(url, sct, `added section w/ id=${sct.id}`, 'addSection');
  }

  /** DELETE: delete the scheme from the server */
  deleteSection (sct: Section | number): Observable<Section> {
    const id = typeof sct === 'number' ? sct : sct.id;
    const url = this.hServ.url(this.sct_s, id);

    return this.hServ.deletePiped<Section>(url, `deleted section id=${id}`, 'deleteSection');
  }
}
