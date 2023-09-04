import {Injectable} from '@angular/core';
import {HttpClient, HttpErrorResponse, HttpHeaders, HttpResponse} from '@angular/common/http';
import {Router} from '@angular/router';
import {TranslateService} from '@ngx-translate/core';
import {Observable, of} from 'rxjs';
import {catchError, flatMap, switchMap, tap} from 'rxjs/operators';

import {
    Chart_old,
    Device,
    Device_Item,
    Device_Item_Group, Device_Item_Type,
    DIG_Param,
    DIG_Param_Type, DIG_Type,
    Disabled_Status,
    Help, Mnemoscheme, Plugin_Type, Saved_User_Chart,
    Scheme_Detail,
    Section,
    Time_Info,
} from './scheme';
import {Connection_State, PaginatorApi, Scheme_Group_Member} from '../user';
import {MessageService} from '../message.service';
import {ISchemeService} from '../ischeme.service';
import {ChangeInfo, ChangeState, Structure_Type} from './settings/settings';
import {TitleService} from '../title.service';

const httpOptions = {
    headers: new HttpHeaders({'Content-Type': 'application/json'})
};

export interface Chart_Value_Data_Item {
    time: number;
    user_id?: number;
    value: any;
    raw?: any;
}

export interface Patch_Structure_Response<T> {
    deleted: number;
    inserted: T[];
    updated: number;
    result: boolean;
}

export interface Chart_Value_Item {
    item_id: number;
    data: Chart_Value_Data_Item[];
}

export interface Paginator_Chart_Value {
    count: number;
    count_all: number;
    results: Chart_Value_Item[];
}

export interface ExportItem {
    id: number;
}

export interface ExportConfig {
    schemes: number[];
    items: ExportItem[];

    ts_from: Date;
    ts_to: Date;
    hide_null: boolean;
}

class DevValues {
    id: number;
    raw_value: any;
    value: any;
}

class StatusItems {
    connection: number;
    items: {
        args: any;
        group_id: number;
        id: number;
        status_id: number;
        title: string;
    }[];
}

export type Modify_Structure_Type = DIG_Type|Device_Item_Type|Device_Item_Group|Section|Device_Item|DIG_Param_Type|DIG_Param|Device|Plugin_Type;

@Injectable()
export class SchemeService extends ISchemeService {
    scheme: Scheme_Detail;

    isSchemeConnected: boolean = false;

    private scheme_s = 'scheme';
    private devValuesInterval: any;
    private scheme2: Scheme_Detail;

    constructor(
        public translate: TranslateService,
        private router: Router,
        http: HttpClient,
        messageService: MessageService,
        private titleService: TitleService,
    ) {
        super(http, messageService);
        // this.scheme = JSON.parse(localStorage.getItem(this.scheme_s));
    }

    updateStatusItems(id: number): Observable<boolean> {
        // get dev values
        return this.http.get<StatusItems>(`/api/v2/scheme/${id}/dig_status/`).pipe(
            switchMap(resp => {
                this.scheme2.conn = of(resp.connection);

                for (const s of this.scheme2.section) {
                    for (const g of s.groups) {
                        g.statuses = [];

                        resp.items.filter(item => item.group_id === g.id).map(item => {
                            const stIns = this.scheme2.dig_status_type.find(sti => sti.id === item.status_id);
                            //const stType = this.scheme2.statusTypes.find(stt => stt.id === stIns.type_id);
                            g.statuses.push({
                                status: stIns,
                                status_id: item.status_id,
                                args: item.args
                            });
                        });

                        this.calculateStatusInfo(g);
                    }
                }
                return of(true);
            }), catchError((error: any): Observable<boolean> => {

                console.error(error); // log to console instead
                this.log(`updateStatusItems failed: ${error.message}`);

                this.scheme2.conn = of(Connection_State.CS_SERVER_DOWN);
                return of(true);
            }));
    }

    updateDevValues(id: number): Observable<boolean> {
        // get dev values
        return this.http.get<DevValues[]>(`/api/v2/scheme/${id}/device_item_value/`).pipe(
            switchMap(resp => {
                // console.log(resp);

                for (const s of this.scheme2.section) {
                    for (const g of s.groups) {
                        for (const di of g.items) {
                            const respItem = resp.find(i => i.id === di.id);

                            if (respItem) {
                                di.val.raw_value = respItem.raw_value;
                                di.val.value = respItem.value;
                            }
                        }
                    }
                }

                return of(true);
            }), catchError(this.handleError('updateDevValues', true)));
    }

    clear(): void {
        localStorage.removeItem(this.scheme_s);
        this.scheme = undefined;

        clearInterval(this.devValuesInterval);
    }

    reloadScheme(scheme_name: string): Observable<boolean> {
        return this.loadScheme(scheme_name, true).pipe(
            flatMap((res) => {
                if (res) {
                    this.scheme = this.scheme2;
                    this.scheme.name = scheme_name;

                    localStorage.setItem(this.scheme_s, JSON.stringify(this.scheme2));
                    this.log('fetched scheme detail');

                    return of(true);
                } else {
                    return of(false);
                }
            }),
        );
    }

    loadScheme2(scheme_name: string, reload?: boolean): Observable<boolean> {
        return this.loadScheme(scheme_name, reload).pipe(
            flatMap((res) => {
                // returns an Observable of type Y
                return res ? this.updateStatusItems(this.scheme2.id) : of(false);
            }),
            flatMap((res) => {
                // console.log(JSON.parse(JSON.stringify(this.scheme2.section)));
                return res ? this.updateDevValues(this.scheme2.id) : of(false);
            }),
            flatMap((res) => {
                if (!res) {
                    return of(false);
                }
                this.scheme = this.scheme2;
                this.scheme.name = scheme_name;

                this.titleService.setVariable('%SCHEME%', this.scheme.title || this.scheme.name);

                localStorage.setItem(this.scheme_s, JSON.stringify(this.scheme2));
                this.log('fetched scheme detail');

                return of(true);
            }),
        );
    }

    loadScheme(scheme_name: string, reload?: boolean): Observable<boolean> {
        if (this.scheme && this.scheme.name == scheme_name && !reload) {
            return of(true);
        }

        this.scheme = undefined; // If comment need compare hash of detail

        const parse_param_value_childs = (group: Device_Item_Group, param_items: DIG_Param_Type[]) => {
            for (const param_value of group.params) {
                for (const param of param_items) {
                    if (param.id === param_value.param_id) {
                        param_value.param = param;
                        break;
                    }
                }
                if (param_value.param.parent_id) {
                    for (const param_value2 of group.params) {
                        if (param_value.param.parent_id == param_value2.param_id) {
                            if (!param_value2.childs) {
                                param_value2.childs = [];
                            }
                            param_value2.childs.push(param_value);
                            break;
                        }
                    }
                }
            }

            for (let index = 0; index < group.params.length; ++index) {
                if (group.params[index].param.parent_id) {
                    group.params.splice(index, 1);
                    --index;
                }
            }
        };

        let lang;
        const match = document.location.pathname.match(/\/(ru|en|fr|es)\//);
        if (match === null) {
            const browserLang = this.translate.getBrowserLang();
            lang = browserLang.match(/ru|en|fr|es/) ? browserLang : 'ru';
        } else {
            lang = match[1];
        }

        return this.httpGet<Scheme_Detail>(`detail/?name=${scheme_name}&lang=${lang}`).pipe(
            switchMap(detail => {
                for (const param of detail.dig_param_type) {
                    if (param.parent_id) {
                        for (const parent_param of detail.dig_param_type) {
                            if (parent_param.id === param.parent_id) {
                                if (parent_param.childs == undefined) {
                                    parent_param.childs = [];
                                }
                                parent_param.childs.push(param);
                                break;
                            }
                        }
                    }
                }

                for (const itemType of detail.device_item_type) {
                    const views = detail.value_view.filter(vv => vv.type_id === itemType.id);
                    if (views && views.length) {
                        itemType.views = views;
                    }

                    for (const sign of detail.sign_type) {
                        if (sign.id === itemType.sign_id) {
                            itemType.sign = sign;
                            break;
                        }
                    }
                }

                for (const status_type of detail.dig_status_type) {
                    for (const status_category of detail.dig_status_category) {
                        if (status_category.id === status_type.category_id) {
                            status_type.category = status_category;
                            break;
                        }
                    }
                }

                const dev_items: Device_Item[] = [];
                for (const dev of detail.device) {
                    for (const item of dev.items) {
                        if (!item.val) {
                            item.val = {raw_value: null, value: null};
                        }

                        for (const itemType of detail.device_item_type) {
                            if (itemType.id === item.type_id) {
                                item.type = itemType;
                                break;
                            }
                        }
                        dev_items.push(item);
                    }
                }

                for (const sct of detail.section) {
                    for (const group of sct.groups) {
                        for (const group_type of detail.dig_type) {
                            if (group_type.id === group.type_id) {
                                group.type = group_type;
                                break;
                            }
                        }

                        if (group.items === undefined) {
                            group.items = [];
                        }

                        for (const item of dev_items) {
                            if (item.group_id === group.id) {
                                group.items.push(item);
                            }
                        }

                        for (const gsts of group.statuses) {
                            for (const sts of detail.dig_status_type) {
                                if (sts.id == gsts.status_id) {
                                    gsts.status = sts;
                                    break;
                                }
                            }
                        }
                        this.calculateStatusInfo(group);
                        parse_param_value_childs(group, detail.dig_param_type);
                    }
                }

                this.scheme2 = detail;
                this.scheme2.name = scheme_name;

                //localStorage.setItem(this.scheme_s, JSON.stringify(detail));
                //this.log('fetched scheme detail');

                // console.log(this.scheme);

                return of(true);
            }),
            catchError(this.handleError('checkCurrentScheme', false))
        );
    }

    public calculateStatusInfo(group: Device_Item_Group): void {
        const strings: string[] = [];
        let str;
        let color = 'green';
        let short_text = 'Ok';
        let last_error_level = 0;

        if (group.statuses === undefined) {
            group.statuses = [];
        }

        for (const sts of group.statuses) {
            if (sts.status.category_id > last_error_level) {
                last_error_level = sts.status.category_id;
                color = sts.status.category.color;
                short_text = sts.status.category.name;
            }
            str = sts.status.text;
            let l = sts.args ? sts.args.length : 0;
            while (l--) {
                str = str.replace('%' + (l + 1), sts.args[l]);
            }
            strings.push(str);
        }

        group.status_info = {color, short_text, text: strings.join('\n')};
    }

    public devItemById(item_id: number): Device_Item {
        for (const dev of this.scheme.device) {
            for (const dev_item of dev.items) {
                if (dev_item.id == item_id) {
                    return dev_item;
                }
            }
        }
        return undefined;
    }

    url(name: string, id?: number): string {
        let url = name;
        if (id !== undefined) {
            url += '/' + id.toString();
        }
        return url + '/?scheme_id=' + this.scheme.id.toString();
    }

    getMembers(): Observable<PaginatorApi<Scheme_Group_Member>> {
        return this.getPiped<PaginatorApi<Scheme_Group_Member>>(this.url('scheme_group'), 'fetched scheme group list', 'getMembers');
    }

    upload_file(item_id: number, file: File): Observable<any> {
        const formData: FormData = new FormData();
        formData.append('fileKey', file, file.name);

        const options = {headers: new HttpHeaders()};
        options.headers.append('Content-Type', 'multipart/form-data');

        const url = this.apiUrl + `write_item_file/?scheme_id=${this.scheme.id}&item_id=${item_id}`;
        return this.http.put(url, formData, options)
            .catch(error => Observable.throw(error));
    }

    get_charts(): Observable<Saved_User_Chart[]> {
        const url = `/api/v2/scheme/${this.scheme.id}/chart/`;
        return this.http.get<Saved_User_Chart[]>(url).pipe(
            catchError((err: HttpErrorResponse) => {
                alert(err.error + '\n' + err.message);
                return of(null as Saved_User_Chart[]);
            }),
        );
    }

    del_chart(chart: Chart_old): Observable<boolean> {
        const url = `/api/v2/scheme/${this.scheme.parent_id || this.scheme.id}/chart/${chart.id}/`;
        return this.http.delete<any>(url).pipe(
            switchMap(resp => of(resp.result)),
            catchError((err: any): Observable<boolean> => {
                alert(err.error + '\n' + err.message);
                return of(false);
            }));
    }

    save_chart(chart: Saved_User_Chart): Observable<Saved_User_Chart> {
        const url = `/api/v2/scheme/${this.scheme.parent_id || this.scheme.id}/chart/`;
        return this.http.put<any>(url, chart)
            .pipe(
                catchError((err: HttpErrorResponse) => {
                        alert(err.error + '\n' + err.message);
                        return of(null as Saved_User_Chart);
                    }),
            );
    }

    getChartData(date_from: number, date_to: number, data: string, limit: number = 1000, offset: number = 0, data_type: string = 'value', bounds_only: boolean = false): Observable<Paginator_Chart_Value> {
        let url = `/api/v2/scheme/${this.scheme.id}/chart_${data_type}/?ts_from=${date_from}&ts_to=${+date_to}&limit=${limit}&offset=${offset}&data=${data}&bounds_only=${+bounds_only}`;
        return this.http.get<Paginator_Chart_Value>(url).pipe(
            tap(
                () => {},
                this.handleError<Paginator_Chart_Value>('chart_' + data_type, undefined),
            ),
        );
        // return this.getPiped<PaginatorApi<Log_Value>>(url, `fetched chart data list`, 'getChartData');
    }

    getChartParamData(date_from: number, date_to: number, data: string, limit: number = 1000, offset: number = 0, bounds_only: boolean = false): Observable<Paginator_Chart_Value> {
        return this.getChartData(date_from, date_to, data, limit, offset, 'param', bounds_only);
    }

    getDisabledStatuses(dig_id: number): Observable<Disabled_Status[]> {
        const url = `/api/v2/scheme/${this.scheme.id}/disabled_status/?dig_id=${dig_id}`;
        return this.http.get<Disabled_Status[]>(url).catch((err: HttpErrorResponse) => of([]));
    }

    delDisabledStatuses(items: Disabled_Status[]): Observable<null> {
        const url = `/api/v2/scheme/${this.scheme.id}/disabled_status/`;
        return this.http.patch<null>(url, items, httpOptions);
    }

    addDisabledStatuses(items: Disabled_Status[]): Observable<null> {
        const url = `/api/v2/scheme/${this.scheme.id}/disabled_status/`;
        return this.http.post<null>(url, items, httpOptions);
    }

    getHelp(): Observable<Help[]> {
        const url = `/api/v2/scheme/${this.scheme.id}/help/`;
        return this.http.get<Help[]>(url).catch((err: HttpErrorResponse) => of([]));
    }

    exportExcel(conf: ExportConfig, path?: string): Observable<HttpResponse<Blob>> {
        if (!path) {
            path = 'excel';
        }

        const url = `/export/${path}/?scheme_id=${this.scheme.id}`;
        const opts = {
            headers: new HttpHeaders({'Content-Type': 'application/json'}),
            observe: 'response' as 'response',
            responseType: 'blob' as 'blob'
        };

        console.log(conf);

        return this.http.post(url, conf, opts).pipe(
            tap(_ => this.log('Export sucessfull')),
            catchError(this.handleError<HttpResponse<Blob>>('exportExcel', undefined))
        );
    }

    copy(scheme_id: number, dry_run: boolean): Observable<any> {
        const url = `/api/v2/scheme/${this.scheme.id}/copy/`;
        return this.http.post<any>(url, {scheme_id, dry_run}).pipe(
            catchError((err: any): Observable<boolean> => {
                alert(err.error + '\n' + err.message);
                return of(undefined);
            }));
    }

    private static getSchemeFieldKey(setting: Structure_Type): keyof Scheme_Detail {
        const keys: Array<Structure_Type> = [
            Structure_Type.ST_DEVICE,
            Structure_Type.ST_SECTION,
            Structure_Type.ST_DEVICE_ITEM_TYPE,
            Structure_Type.ST_DIG_PARAM_TYPE,
            Structure_Type.ST_DIG_TYPE,
            Structure_Type.ST_DIG_STATUS_TYPE,
            Structure_Type.ST_DIG_STATUS_CATEGORY,
            Structure_Type.ST_DIG_MODE_TYPE,
            Structure_Type.ST_VALUE_VIEW,
        ];
        return keys.includes(setting) ? setting as keyof Scheme_Detail : null;
    }

    public remove_structure<T extends Modify_Structure_Type>(settingName: Structure_Type, item: T): Observable<Patch_Structure_Response<T>> {
        return this.modify_structure<T>(settingName, [{
            state: ChangeState.Delete,
            obj: item,
        }]);
    }

    public upsert_structure<T extends Modify_Structure_Type>(settingName: Structure_Type, item: T|Omit<T,'id'>, prev?: T): Observable<Patch_Structure_Response<T>> {
        return this.modify_structure<T>(settingName, [{
            state: ChangeState.Upsert,
            obj: item,
            prev,
        }]);
    }

    public modify_structure<T extends Modify_Structure_Type, Y=Omit<T, 'id'>>(settingName: Structure_Type, data: ChangeInfo<T | Y>[]): Observable<Patch_Structure_Response<T>> {
        let dataToUpdate: (Y | T | { id: number })[] = [];
        for (const item of data) {
            if (item.state === ChangeState.Delete) {
                dataToUpdate.push({ id: (item.obj as T).id });
            } else if (item.state === ChangeState.Upsert) {
                const obj = {...item.obj};
                for (const n in obj as any)
                    if (typeof obj[n] === 'object' && obj[n] !== null)
                        delete obj[n];

                dataToUpdate.push(obj);
            }
        }

        if (dataToUpdate.length === 0) return of(null);

        const url = `/api/v2/scheme/${this.scheme.id}/structure/${settingName}/`;
        return this.http.patch<Patch_Structure_Response<T>>(url, dataToUpdate)
            .pipe(tap((response) => {
                if (!response.result) return;

                const findObj = (array: Array<T>, item: T, ignoreKeys = ['id']) => {
                    return array.find((i) => {
                        const keys = this.diff(item, i);
                        return keys.filter(key => ignoreKeys.indexOf(key as string) === -1).length === 0;
                    }) as T;
                };

                const findGroup = (id: number) => {
                    return this.scheme.section.map(sct => sct.groups)
                        .reduce((prev, curr) => {
                            prev.push(...curr);
                            return prev;
                        }, [])
                        .find(group => id === group.id);
                };

                const recursiveSearchParam = (array: Array<DIG_Param>, item: DIG_Param): DIG_Param => {
                    for (let param of array) {
                        if (param.id === item.id
                         || (param = recursiveSearchParam(param.childs || [], item)))
                            return param;
                    }
                    return null;
                };

                const recursiveSearchParamByTypeId = (array: Array<DIG_Param>, typeId: number): DIG_Param => {
                    for (let param of array) {
                        if (param.param_id === typeId
                         || (param = recursiveSearchParamByTypeId(param.childs || [], typeId)))
                             return param;
                    }
                    return null;
                };

                const findAndUpdate = (array: Array<T>, item: T): boolean => {
                    let itemToUpdate;
                    if (item instanceof DIG_Param) {
                        itemToUpdate = recursiveSearchParam(array as Array<DIG_Param>, item);
                    } else {
                        itemToUpdate = array.find(({ id }) => item.id === id);
                    }

                    if (!itemToUpdate) {
                        return false;
                    }

                    Object.assign(itemToUpdate, item);
                    return true;
                };

                const findAndRemove = (array: Array<T>, item: T) => {
                    const itemIdx = array.findIndex(({ id }) => item.id === id);
                    if (itemIdx >= 0) {
                        array.splice(itemIdx, 1);
                    }
                };

                const findDevice = (device_id: number) => {
                    return this.scheme.device.find(dev => dev.id === device_id);
                };

                const findSection = (section_id: number) => {
                    return this.scheme.section.find(sct => sct.id === section_id);
                };

                data.forEach((changeInfo) => {
                    let array: Array<T>;
                    let array2: Array<T>;

                    const { obj, prev, state } = changeInfo;

                    if (settingName === Structure_Type.ST_DEVICE_ITEM_GROUP) {
                        array = findSection((<Device_Item_Group>obj).section_id)?.groups as any;
                    } else if (settingName === Structure_Type.ST_DEVICE_ITEM) {
                        const devItem = obj as Device_Item;
                        array = findGroup(devItem.group_id).items as any;
                        array2 = findDevice(devItem.device_id).items as any;
                    } else if (settingName === Structure_Type.ST_DIG_PARAM) {
                        array = findGroup((<Omit<DIG_Param, 'value'>>obj).group_id).params as any;
                        const param = <Omit<DIG_Param, 'value'>>obj;

                        if (!param.param) {
                            param.param = this.scheme.dig_param_type.find((t) => t.id === param.param_id);
                        }

                        if (param.param.parent_id) {
                            const parentParam = recursiveSearchParamByTypeId(array as Array<DIG_Param>, param.param.parent_id);
                            array = parentParam.childs as T[];
                        }
                    } else if (settingName === Structure_Type.ST_TRANSLATION) {
                        return;
                    } else if (settingName === Structure_Type.ST_CODE_ITEM) {
                        return;
                    } else {
                        const key = SchemeService.getSchemeFieldKey(settingName);

                        array = this.scheme[key] as Array<T>;
                        array2 = undefined;
                    }

                    if (state === ChangeState.Delete) {
                        findAndRemove(array, obj as T);
                        array2 && findAndRemove(array2, obj as T);
                    } else if (state === ChangeState.Upsert) {
                        if ((<T>obj).id) {
                            if (!findAndUpdate(array, obj as T)) {
                                let arrayFromRemove: Array<T>;
                                switch (settingName) {
                                    case Structure_Type.ST_DIG_PARAM: {
                                        const param = <Omit<DIG_Param, 'value'>>prev;
                                        arrayFromRemove = findGroup(param.group_id)?.params as any;

                                        if (param.param.parent_id) {
                                            const parentParam = recursiveSearchParamByTypeId(arrayFromRemove as Array<DIG_Param>,
                                                param.param.parent_id);

                                            arrayFromRemove = parentParam.childs as T[];
                                        }
                                        break;
                                    }
                                    case Structure_Type.ST_DEVICE_ITEM_GROUP:
                                        arrayFromRemove = findSection((<Device_Item_Group>prev).section_id)?.groups as any;
                                        break;
                                    case Structure_Type.ST_DEVICE_ITEM:
                                        arrayFromRemove = findGroup((<Device_Item>prev).group_id)?.items as any;
                                        break;
                                }

                                if (arrayFromRemove) {
                                    findAndRemove(arrayFromRemove, <T>prev);
                                }

                                array.push(obj as T);
                            }

                            if (array2 && settingName === Structure_Type.ST_DEVICE_ITEM) {
                                if (!findAndUpdate(array2, obj as T)) {
                                    findAndRemove(
                                        findDevice((<Device_Item>prev).device_id).items as any,
                                        prev as T,
                                    );

                                    array2.push(obj as T);
                                }
                            }
                        } else {
                            const tObj = obj as T;
                            const insertedItem = findObj(response.inserted, tObj, ['parent_id', 'scheme_id', 'value', 'id']);
                            tObj.id = insertedItem.id;
                            array.push(tObj);
                            array2 && array2.push(tObj);
                        }
                    }
                });
            }));
    }

    diff<T>(a: T, b: T): (keyof T)[] {
        return Object.keys(a)
            .filter(key => typeof a[key] !== 'object' && typeof b[key] !== 'object')
            .filter(key => a[key] !== b[key]) as (keyof T)[];
    }

    getMnemoscheme(): Observable<Mnemoscheme[]> {
        return this.http.get<Mnemoscheme[]>(`/api/v2/scheme/${this.scheme.id}/mnemoscheme/`);
    }

    getMnemoschemeImage(id: number): Observable<string> {
        return this.http.get(`/api/v2/scheme/${this.scheme.id}/mnemoscheme/${id}/`, {
            responseType: 'text',
        });
        // return this.http.get('/assets/img/salat.svg', { responseType: 'text' }); // for testing
    }

    getTimeInfo(): Observable<Time_Info> {
        return this.http.get<Time_Info>(`/api/v2/scheme/${this.scheme.id}/time_info/`);
    }
}
