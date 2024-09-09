import {
    AfterViewInit,
    ChangeDetectorRef,
    Component,
    ComponentFactoryResolver,
    ComponentRef,
    ElementRef,
    OnDestroy,
    OnInit,
    ViewChild,
    ViewContainerRef
} from '@angular/core';
import { HttpClient } from '@angular/common/http';
import {PageEvent} from '@angular/material/paginator';
import {MatSort} from '@angular/material/sort';
import {MatTableDataSource} from '@angular/material/table';
import {TranslateService} from '@ngx-translate/core';
import {ActivatedRoute} from '@angular/router';
import {CookieService} from 'ngx-cookie-service';
import {combineLatest, Observable, Subject, Subscription, SubscriptionLike} from 'rxjs';
import {debounceTime, map, tap} from 'rxjs/operators';

import {
    Device_Item,
    Device_Item_Group,
    DIG_Param,
    Log_Base,
    Log_Event,
    Log_Event_Type,
    Log_Mode,
    Log_Param,
    Log_Status,
    Log_Status_Direction,
    Log_Value,
    Section
} from '../scheme';
import {Scheme_Group_Member} from '../../user';
import {AuthService} from '../../auth/service';
import {SchemeService} from '../scheme.service';
import {ControlService, WebSockCmd} from '../control.service';
import {NeedSidebar, SidebarService} from '../sidebar.service';
import {
    DigLogFilter,
    LogFilter,
    LogsFilter,
    LogSidebarComponent,
    ParamsLogFilter,
    SelectedLogs, StatusLogFilter,
    ValuesLogFilter
} from './log-sidebar/log-sidebar.component';
import {LoadingProgressbar} from '../loading-progressbar/loading.progressbar';
import {MatSnackBar} from '@angular/material/snack-bar';
import {VideoStreamDialogComponent} from '../dev-item-value/video-stream-dialog/video-stream-dialog.component';
import {MatDialog} from '@angular/material/dialog';

interface LogItem {
    type_id: string;
    user_id: number;
    text: string;
    time: number;

    color?: string;
    bgColor?: string;
    item?: Device_Item;
    raw_value?: string;
}

interface LogTableItem extends LogItem {
    date: Date;
    color: string;
    bgColor: string;
}

@Component({
    selector: 'app-log',
    templateUrl: './log.component.html',
    styleUrls: ['./log.component.css']
})
export class LogComponent extends LoadingProgressbar implements OnInit, AfterViewInit, OnDestroy, NeedSidebar {
    displayedColumns = ['user', 'timestamp_msecs', 'message'];
    logDatabase: LogHttpDao | null;
    dataSource = new MatTableDataSource();
    private dataTimeBounds: Record<LogItem['type_id'], { min: number, max: number }> = {};

    isRateLimitReached = false;

    canExecScript: boolean;

    itemsPerPage;

    sub: SubscriptionLike;

    members: Scheme_Group_Member[] = [];

    @ViewChild('table', {read: ElementRef}) tableElem: ElementRef;
    @ViewChild(MatSort, {static: true}) sort: MatSort;

    private scrollSubject: Subject<number> = new Subject();
    scrollHandler = (ev) => {
        const {scrollTop} = document.documentElement;
        if (document.body.scrollHeight - scrollTop <= 700) {
            this.scrollSubject.next(scrollTop);
        }
    };

    cmd = '';
    private sidebarRef: ComponentRef<LogSidebarComponent>;
    private sidebarActionBroadcast$: Subscription;
    private isFirstRequest = true;
    private currentFilter: LogsFilter;
    private scrollEvent$: Subscription;
    private log_mode$: Subscription;
    private log_status$: Subscription;
    private log_param$: Subscription;
    private data: LogItem[];

    constructor(
        public translate: TranslateService,
        private controlService: ControlService,
        private user: AuthService,
        private schemeService: SchemeService,
        private http: HttpClient,
        private activatedRoute: ActivatedRoute,
        public cookie: CookieService,
        private resolver: ComponentFactoryResolver,
        private sidebarService: SidebarService,
        private dialog: MatDialog,
        snackBar: MatSnackBar,
        changeDetectorRef: ChangeDetectorRef,
    ) {
        super(snackBar, changeDetectorRef);

        this.activatedRoute.queryParams.subscribe(params => {
            if (params['cmd']) {
                this.cmd = params['cmd'];
            }
        });

        this.canExecScript = this.user.checkPermission('exec_script');
        this.schemeService.getMembers().subscribe(members => this.members = members.results);

        this.logDatabase = new LogHttpDao(this.http, this.schemeService);
    }

    getSidebarWidget(viewContainerRef: ViewContainerRef): ComponentRef<LogSidebarComponent> {
        const factory = this.resolver.resolveComponentFactory(LogSidebarComponent);
        this.sidebarRef = viewContainerRef.createComponent(factory);
        return this.sidebarRef;
    }

    ngOnInit() {
        this.sidebarService.resetContent();
        this.sidebarActionBroadcast$ = this.sidebarService.getContentActionBroadcast()
            .subscribe((contentAction) => {
                this.isFirstRequest = true;
                this.updateFilter(contentAction.data, false);
            });

        this.sub = this.controlService.byte_msg.subscribe((msg) => {
            if (msg.cmd !== WebSockCmd.WS_EVENT_LOG) {
                return;
            }

            if (msg.data === undefined) {
                console.warn('Log_Event without data');
                return;
            }

            const rows = this.controlService.parseEventMessage(msg.data);
            const logItems = rows.map(row => this.logDatabase.mapLogEvent(row));
            logItems.forEach((logItem: LogTableItem) => {
                logItem.date = new Date();
                logItem.date.setTime(logItem.time);
            });
            logItems.sort((a, b) => b.time - a.time);
            this.dataSource.data = [...logItems, ...this.dataSource.data];
        });

        this.log_mode$ = this.controlService.log_mode.subscribe(
            this.logsProcessor(this.logDatabase.mapLogMode, 'mode'),
        );

        this.log_status$ = this.controlService.log_status.subscribe(
            this.logsProcessor(this.logDatabase.mapLogStatus, 'status'),
        );

        this.log_param$ = this.controlService.log_param.subscribe(
            this.logsProcessor(this.logDatabase.mapLogParam, 'param'),
        );

        this.scrollEvent$ = this.scrollSubject.asObservable()
            .pipe(debounceTime(300))
            .subscribe(() => {
                if (this.loading) {
                    return;
                }
                // смотреть на min(date), max(date) разных журналов и "догружать" их до минимальных/максимальных.
                // Если таких нет, то догружать новые.
                this.startLoading();

                const observables: Array<Observable<LogItem[]>> = [];
                const schemeId = this.schemeService.scheme.id;
                const logFilter: LogFilter = LogComponent.getLogFilter(this.currentFilter, 50);

                let minBound, minBoundKey;
                Object.keys(this.dataTimeBounds) // собираем границы диапазона ts_from, ts_to
                    .forEach((key: string) => {
                        const {min, max} = this.dataTimeBounds[key];

                        if (minBound === undefined || minBound > min) {
                            minBound = min;
                            minBoundKey = key;
                        }
                    });

                let loadAll = true;
                const ts: Record<LogItem['type_id'], { ts_from: LogFilter['ts_from'], ts_to: LogFilter['ts_to'] }> = {};

                // найти типы, для которых недостаточно данных
                Object.keys(this.dataTimeBounds)
                    .forEach((key: string) => {
                        const {min, max} = this.dataTimeBounds[key];

                        let ts_from, ts_to;

                        if (min - minBound > 10000) { // если разница больше 10 секунд, значит, надо догрузить данных
                            ts_from = minBound;
                            ts_to = min;
                        }

                        if (ts_from && ts_to) {
                            ts[key] = {ts_from, ts_to};
                            loadAll = false;
                        }
                    });

                if (!loadAll) {
                    if (this.currentFilter.selectedLogs.event && ts.event) {
                        observables.push(this.logDatabase.getEvents(schemeId, {...logFilter, ...ts.event}));
                    }
                    if (this.currentFilter.selectedLogs.mode && ts.mode) {
                        observables.push(this.logDatabase.getModes(schemeId, {
                            ...logFilter,
                            ...ts.mode,
                            dig_id: this.currentFilter.selectedGroupsId,
                        }));
                    }
                    if (this.currentFilter.selectedLogs.param && ts.param) {
                        observables.push(this.logDatabase.getParams(schemeId, {
                            ...logFilter,
                            ...ts.param,
                            dig_param_id: this.currentFilter.selectedParamsId,
                        }));
                    }
                    if (this.currentFilter.selectedLogs.status && ts.status) {
                        observables.push(this.logDatabase.getStatuses(schemeId, {
                            ...logFilter,
                            ...ts.status,
                            dig_id: this.currentFilter.selectedGroupsId,
                            status_id: this.currentFilter.selectedStatusesId,
                        }));
                    }
                    if (this.currentFilter.selectedLogs.value && ts.value) {
                        observables.push(this.logDatabase.getValues(schemeId, {
                            ...logFilter,
                            ...ts.value,
                            item_id: this.currentFilter.selectedItemsId,
                        }));
                    }
                    this.processResponseObservables(observables, true)
                        .subscribe((count) => {
                            if (count < 2) {
                                this.updateFilter({
                                    ...this.currentFilter,
                                    ts_to: minBound,
                                    ts_from: 0,
                                }, true, 50);
                            }
                        });
                } else {
                    this.updateFilter({
                        ...this.currentFilter,
                        ts_to: minBound,
                        ts_from: 0,
                    }, true, 50);
                }
            });
    }

    ngAfterViewInit() {
        window.document.addEventListener('scroll', this.scrollHandler);
    }

    getUserName(id: number): string {
        if (id === null || id === 0) {
            return '';
        }
        for (const user of this.members) {
            if (user.id === id) {
                return user.name;
            }
        }
        return this.translate.instant('LOG.UNKNOWN_USER') + ' ' + String(id);
    }

    ngOnDestroy() {
        this.sub.unsubscribe();
        this.sidebarActionBroadcast$.unsubscribe();
        this.sidebarService.resetSidebar();
        this.sidebarService.resetContent();

        window.document.removeEventListener('scroll', this.scrollHandler);
    }

    dateFormat(cell: any): string {
        if (cell.clientWidth <= 60) {
            return 'dd H:m';
        }
        return 'dd.MM.yy HH:mm:ss';
    }

    getColor(eventType: number): string {
        switch (eventType) {
            case Log_Event_Type.ET_DEBUG:
                return '#5A9740';
            case Log_Event_Type.ET_WARNING:
                return '#A39242';
            case Log_Event_Type.ET_CRITICAL:
                return '#994242';
            case Log_Event_Type.ET_INFO:
                return '#407D9E';
        }
        return 'black';
    }

    execScript(script: string): void {
        this.controlService.execScript(script);
    }

    handlePage($event: PageEvent) {
        console.log($event);
        const pi = $event.pageIndex;
        const ppi = $event.previousPageIndex;

        if (pi > ppi) {
            // scroll top
            window.scrollTo(0, 0);
        } else if (pi < ppi) {
            // scroll bottom
            window.scrollTo(0, document.body.scrollHeight);
        }

        if ($event.pageSize != this.itemsPerPage) {
            this.itemsPerPage = $event.pageSize;
            this.cookie.set('logItemsPerPage', String($event.pageSize), 365, '/');
        }
    }

    openImg(row: any): void {
        let settings = VideoStreamDialogComponent.get_default_settings();
        settings['data'] = {isImg: true, devItem: null, img: row};
        let dialogRef = this.dialog.open(VideoStreamDialogComponent, settings);

        dialogRef.afterClosed().subscribe(result => console.log(result));
    }

    private updateFilter(data: LogsFilter, append: boolean, limit?: number) {
        this.startLoading();
        this.currentFilter = data;

        const observables: Array<Observable<LogItem[]>> = [];
        const schemeId = this.schemeService.scheme.id;
        const logFilter: LogFilter = LogComponent.getLogFilter(data, limit);

        if (data.selectedLogs.event) {
            observables.push(this.logDatabase.getEvents(schemeId, {
                ...logFilter,
                selectedTextEvents: this.currentFilter.selectedTextEvents,
            }));
        }
        if (data.selectedLogs.mode) {
            observables.push(this.logDatabase.getModes(schemeId, {
                ...logFilter,
                dig_id: data.selectedGroupsId,
            }));
        }
        if (data.selectedLogs.param) {
            observables.push(this.logDatabase.getParams(schemeId, {
                ...logFilter,
                dig_param_id: data.selectedParamsId,
            }));
        }
        if (data.selectedLogs.status) {
            observables.push(this.logDatabase.getStatuses(schemeId, {
                ...logFilter,
                dig_id: data.selectedGroupsId,
                status_id: data.selectedStatusesId,
            }));
        }
        if (data.selectedLogs.value) {
            observables.push(this.logDatabase.getValues(schemeId, {
                ...logFilter,
                item_id: data.selectedItemsId,
            }));
        }

        this.processResponseObservables(observables, append)
            .subscribe(() => {
                if (this.isFirstRequest) {
                    this.isFirstRequest = false;
                    const dataTypesCount = Object.keys(this.currentFilter.selectedLogs)
                        .filter(key => this.currentFilter.selectedLogs[key])
                        .length;

                    if (this.dataSource.data.length < 50) {
                        this.updateFilter({
                            ...this.currentFilter,
                            ts_to: this.currentFilter.ts_from,
                            ts_from: 0,
                        }, true, Math.round(50 / dataTypesCount));
                    }
                }
            });
    }

    private static getLogFilter(data: LogsFilter, limit: number) {
        const logFilter: LogFilter = {
            ts_from: data.ts_from,
            ts_to: data.ts_to,
        };

        if (data.filter) {
            logFilter.filter = data.filter;
            logFilter.case_sensitive = data.case_sensitive;
        }

        if (limit) {
            logFilter.limit = limit;
        }

        return logFilter;
    }

    private processResponseObservables(observables: Array<Observable<LogItem[]>>, append: boolean): Observable<number> {
        if (!append) {
            this.dataTimeBounds = {};
        }

        return combineLatest(observables)
            .pipe(
                map(logEvents => logEvents.reduce((prev, curr) => prev.concat(curr))),
                tap(logEvents => logEvents.forEach((logItem: LogTableItem) => {
                    // set data bounds
                    const bounds = this.dataTimeBounds[logItem.type_id];
                    if (!bounds) {
                        this.dataTimeBounds[logItem.type_id] = {min: logItem.time, max: logItem.time};
                    } else {
                        if (bounds.min > logItem.time) {
                            bounds.min = logItem.time;
                        }

                        if (bounds.max < logItem.time) {
                            bounds.max = logItem.time;
                        }
                    }

                    // set up date
                    logItem.date = new Date();
                    logItem.date.setTime(logItem.time);
                })),
                map((logEvents) => {
                    this.data = append ? [...this.data, ...logEvents] : logEvents;
                    if (this.data.length > 0) {
                        this.data.sort((a: LogItem, b: LogItem) => b.time - a.time);
                        const {min} = Object
                            .keys(this.dataTimeBounds)
                            .map(key => this.dataTimeBounds[key])
                            .reduce((prev, curr) => {
                                if (prev.min > curr.min) {
                                    return prev;
                                }

                                return curr;
                            });

                        this.dataSource.data = this.data.filter((item: LogItem) => item.time > min);
                    } else {
                        this.dataSource.data = this.data;
                    }
                    this.finishedLoading();

                    return logEvents.length;
                }, (error) => this.errorLoading(error)));
    }

    private logsProcessor<T extends Log_Base>(mapper: (logs: T) => LogItem, flagName: keyof SelectedLogs) {
        const bindedMapper = mapper.bind(this.logDatabase) as (log: T) => LogItem;
        return (logs: T[]) => {
            if (!this.currentFilter.selectedLogs[flagName]) {
                return;
            }

            const logItems = logs.map(bindedMapper).sort((a, b) => b.time - a.time);
            logItems.forEach((logItem: any) => {
                logItem.date = new Date();
                logItem.date.setTime(logItem.time);
            });
            this.dataSource.data = [...logItems, ...this.dataSource.data];
        };
    }
}

/** An example database that the data source uses to retrieve data for the table. */
export class LogHttpDao {
    constructor(private http: HttpClient, private schemeService: SchemeService) {
    }

    getEvents(schemeId: number, filter: LogFilter & { selectedTextEvents?: Array<number> }): Observable<LogItem[]> {
        const copy = {...filter};
        if (filter.selectedTextEvents) {
            copy.type_id = filter.selectedTextEvents.join(',');
        }
        delete copy.selectedTextEvents;

        return this.request<Log_Event>('event', schemeId, copy)
            .pipe(map((logEvents) => logEvents.map((logEvent) => this.mapLogEvent(logEvent))));
    }

    public mapLogEvent(logEvent: Log_Event): LogItem {
        let color;
        switch (logEvent.type_id) {
            case Log_Event_Type.ET_DEBUG:
                color = '#5A9740';
                break;
            case Log_Event_Type.ET_WARNING:
                color = '#A39242';
                break;
            case Log_Event_Type.ET_CRITICAL:
                color = '#994242';
                break;
            case Log_Event_Type.ET_INFO:
                color = '#407D9E';
                break;
            default:
                color = 'black';
        }

        return {
            type_id: 'event',
            text: `[${logEvent.category}] ${logEvent.text}`,
            time: +logEvent.timestamp_msecs,
            user_id: logEvent.user_id,
            color,
        };
    }

    getModes(schemeId: number, filter: DigLogFilter): Observable<LogItem[]> {
        return this.request<Log_Mode>('mode', schemeId, filter)
            .pipe(
                map((logMode) => logMode.map(mode => this.mapLogMode(mode))),
            );
    }

    public mapLogMode(logMode: Log_Mode): LogItem {
        return {
            type_id: 'mode',
            text: `${this.get_dig_title(logMode.group_id)} = ${this.get_mode_title(logMode.mode_id)}`,
            time: +logMode.timestamp_msecs,
            user_id: logMode.user_id,
            bgColor: '#A1FFBA',
        };
    }

    getStatuses(schemeId: number, filter: StatusLogFilter): Observable<LogItem[]> {
        return this.request<Log_Status>('status', schemeId, filter)
            .pipe(
                map((logStatuses) => logStatuses.map((logStatus) => this.mapLogStatus(logStatus))),
            );
    }

    public mapLogStatus(logStatus: Log_Status): LogItem {
        const {text, color} = this.getStatusText(logStatus);
        return {
            type_id: 'status',
            user_id: logStatus.user_id,
            time: +logStatus.timestamp_msecs,
            text,
            color,
            bgColor: '#A4EEFF',
        };
    }

    getParams(schemeId: number, filter: ParamsLogFilter): Observable<LogItem[]> {
        return this.request<Log_Param>('param', schemeId, filter)
            .pipe(
                map((logParams) => logParams.map(logParam => this.mapLogParam(logParam))),
            );
    }

    public mapLogParam(logParam: Log_Param): LogItem {
        return {
            type_id: 'param',
            time: +logParam.timestamp_msecs,
            text: `[param] ${this.get_dig_param_name(logParam.group_param_id)} = ${logParam.value}`,
            user_id: logParam.user_id,
            bgColor: '#FFD0A1',
        };
    }

    getValues(schemeId: number, filter: ValuesLogFilter): Observable<LogItem[]> {
        return this.request<Log_Value>('value', schemeId, filter)
            .pipe(
                map((logValues) => logValues.map((logValue) => this.mapLogValue(logValue))),
            );
    }

    public mapLogValue(logValue: Log_Value): LogItem {
        const isBlob = LogHttpDao.is_blob(logValue.raw_value);
        const devItemName = this.getDevItemName(logValue.item_id);

        let text: string;
        let raw_value: string;
        let item: Device_Item;

        if (!isBlob) {
            text = `${devItemName} ${logValue.value}`;
            if (logValue.raw_value !== null) {
                text += ` (${logValue.raw_value})`;
            }

            raw_value = null;
        } else {
            text = '';
            raw_value = logValue.raw_value;
            item = this.getDevItem(logValue.item_id);
        }

        return {
            type_id: 'value',
            user_id: logValue.user_id,
            time: +logValue.timestamp_msecs,
            text,
            raw_value,
            item,
            bgColor: '#DAFFA1',
        };
    }

    private request<T>(type: string, schemeId: number, filter: LogFilter): Observable<T[]> {
        const requestUrl = this.getUrl(type, schemeId, filter);
        return this.http.get<T[]>(requestUrl);
    }

    private getUrl(type: string, schemeId: number, filter: Object): string {
        let href = `/api/v2/scheme/${schemeId}/log/${type}/`;
        let isFirst = true;

        Object.keys(filter).forEach((key) => {
            const v = filter[key];
            if (isFirst) {
                href += `?${key}=${v}`;
                isFirst = false;
            } else {
                href += `&${key}=${v}`;
            }
        });

        return href;
    }

    private getDevItem(item_id: number): Device_Item {
        let devItem: Device_Item;
        this.schemeService.scheme.device.find((dev) => {
            devItem = dev.items.find(item => item.id === item_id);
            return !!devItem;
        });

        return devItem;
    }

    private getDevItemName(item_id: number): string {
        let devItem: Device_Item;
        this.schemeService.scheme.device.find((dev) => {
            devItem = dev.items.find(item => item.id === item_id);
            return !!devItem;
        });

        if (!devItem) {
            throw new Error(`Device Item ${item_id} not found`);
        }

        const digTitle = this.get_dig_title(devItem.group_id);
        return `${digTitle}->${devItem.name || devItem.type.title}`;
    }

    private get_dig_param_name(param_id: number): string {
        let group: Device_Item_Group;
        let paramPath: DIG_Param[];

        const section = this.schemeService.scheme.section
            .find((section) => {
                group = section.groups.find((g): boolean => {
                    paramPath = this.recursiveSearchParamNames(g.params, param_id);
                    return !!paramPath;
                });

                return !!group;
            });

        if (!group || !paramPath) {
            throw new Error(`Param ${param_id} not found`);
        }

        let paramPathString = paramPath.map((param) => param.param.title).join('->');
        paramPathString = `${this.groupTitle(section, group)}->${paramPathString}`;

        return paramPathString;
    }

    private recursiveSearchParamNames(params: DIG_Param[], param_id: number): DIG_Param[] {
        let param = params.find(para => para.id === param_id);
        if (!param) {
            let paramPath: DIG_Param[];
            const parentParam = params.find((para) => {
                if (!para.childs) {
                    return false;
                }

                paramPath = this.recursiveSearchParamNames(para.childs, param_id);
                return !!paramPath;
            });
            if (paramPath) {
                return [parentParam, ...paramPath];
            }
        }

        if (!param) {
            return null;
        }

        return [param];
    }

    private get_dig_title(group_id: number) {
        let group: Device_Item_Group;
        const section = this.schemeService.scheme.section
            .find((sect) => {
                group = sect.groups.find(gr => gr.id === group_id);
                return !!group;
            });

        if (!group || !section) {
            throw new Error(`Group ${group_id} not found`);
        }

        return this.groupTitle(section, group);
    }

    private get_mode_title(mode_id: number) {
        const mode = this.schemeService.scheme.dig_mode_type.find(m => m.id === mode_id);
        return mode.title;
    }

    private getStatusText(logStatus: Log_Status): { color: string, text: string } {
        const status = this.schemeService.scheme.dig_status_type.find(status => status.id === logStatus.status_id);
        const digTitle = this.get_dig_title(logStatus.group_id);

        let direction: string;
        let color = null;
        let emoji: string;
        if (logStatus.direction === Log_Status_Direction.SD_DEL) {
            direction = '-';
            emoji = '🆙 ';
        } else {
            const category = this.schemeService.scheme.dig_status_category.find(cat => cat.id === status.category_id);
            emoji = LogHttpDao.getEmoji(category.name);
            color = LogHttpDao.getStatusTextColor(category.name);
            direction = '+';
        }

        const formattedStatusText = LogHttpDao.formatStatusText(status.text, logStatus.args);
        const text = `${digTitle} ${emoji} ${formattedStatusText} ${direction}`;

        return {text, color};
    }

    private static formatStatusText(text, args): string {
        if (args !== null) {
            const args_list = args.split(',');
            args_list.forEach((arg, idx) => {
                text = text.replaceAll('%' + (idx + 1), arg);
            });
        }
        return text;
    }

    private static getEmoji(categoryName: string): string {
        let emoji: string;
        switch (categoryName) {
            case 'Error':
                emoji = '🚨 ';
                break;
            case 'Warn':
                emoji = '⚠️ ';
                break;
            case 'Ok':
                emoji = '✅ ';
                break;
            default:
                emoji = '';
        }

        return emoji;
    }

    private static getStatusTextColor(name: string): string {
        let color: string;
        switch (name) {
            case 'Error':
                color = '#994242';
                break;
            case 'Warn':
                color = '#A39242';
                break;
            case 'Ok':
                color = '#5A9740';
                break;
            default:
                color = null;
        }

        return color;
    }

    private groupTitle(section: Section, group: Device_Item_Group): string {
        let group_title = group.title || group.type.title;
        if (this.schemeService.scheme.section.length > 0) {
            group_title = `${section.name}->${group_title}`;
        }

        return group_title;
    }

    private static is_blob(raw: any) {
        return raw && typeof raw === 'string' && raw.startsWith('img:');
    }
}

