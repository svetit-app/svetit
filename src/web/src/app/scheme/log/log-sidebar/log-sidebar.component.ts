import { Component, OnInit, inject } from '@angular/core';
import {SchemeService} from '../../scheme.service';
import {DropdownSettings} from 'angular2-multiselect-dropdown/lib/multiselect.interface';
import {Device_Item_Group, DIG_Param} from '../../scheme';
import { UntypedFormControl, ReactiveFormsModule, FormsModule } from '@angular/forms';
import * as moment from 'moment';
import {DateAdapter, MAT_DATE_FORMATS, MAT_DATE_LOCALE} from '@angular/material/core';
import {MAT_MOMENT_DATE_ADAPTER_OPTIONS, MAT_MOMENT_DATE_FORMATS, MomentDateAdapter} from '@angular/material-moment-adapter';
import {SidebarService} from '../../sidebar.service';
import { MatFormField, MatSuffix, MatHint } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatDatepickerInput, MatDatepickerToggle, MatDatepicker } from '@angular/material/datepicker';
import { MatCheckbox } from '@angular/material/checkbox';
import { AngularMultiSelectModule } from 'angular2-multiselect-dropdown';
import { MatButton } from '@angular/material/button';

type DIG_DropdownData = { folderName: string, label: string, value: number };

function parseDate(date: UntypedFormControl, time: string): number {
    let time_arr = time.split(':');
    let date_from = date.value.toDate();
    date_from.setHours(+time_arr[0], +time_arr[1], +time_arr[2] || 0);

    return date_from.getTime();
}

function zero(n: number): string {
    return n >= 10 ? `${n}` : `0${n}`;
}

function parseDateToDateAndTime(date: number, fcRef: UntypedFormControl): string {
    const d = new Date(date);

    fcRef.setValue(moment(d));

    return `${zero(d.getHours())}:${zero(d.getMinutes())}:${zero(d.getSeconds())}`;
}

export interface SelectedLogs {
    event: boolean;
    mode: boolean;
    param: boolean;
    status: boolean;
    value: boolean;
}

export interface LogsFilter {
    ts_from: number;
    ts_to: number;
    selectedLogs: SelectedLogs;
    selectedTextEvents: Array<number>;
    filter: string;
    case_sensitive: boolean;
    selectedGroupsId: Array<number>;
    selectedItemsId: Array<number>;
    selectedParamsId: Array<number>;
    selectedStatusesId: Array<number>;
}

export interface LogFilter {
    ts_from: LogsFilter['ts_from'];
    ts_to: LogsFilter['ts_to'];
    limit?: number;
    filter?: LogsFilter['filter'];
    case_sensitive?: LogsFilter['case_sensitive'];
    type_id?: string;
}

export interface DigLogFilter extends LogFilter {
    dig_id: LogsFilter['selectedGroupsId'];
}

export interface StatusLogFilter extends DigLogFilter {
    status_id: LogsFilter['selectedStatusesId'];
}

export interface ParamsLogFilter extends LogFilter {
    dig_param_id: LogsFilter['selectedParamsId'];
}

export interface ValuesLogFilter extends LogFilter {
    item_id: LogsFilter['selectedItemsId'];
}

@Component({
    selector: 'app-log-sidebar',
    templateUrl: './log-sidebar.component.html',
    styleUrls: ['./log-sidebar.component.css'],
    providers: [
        // The locale would typically be provided on the root module of your application. We do it at
        // the component level here, due to limitations of our example generation script.
        { provide: MAT_DATE_LOCALE, useValue: 'ru-RU' },
        // `MomentDateAdapter` and `MAT_MOMENT_DATE_FORMATS` can be automatically provided by importing
        // `MatMomentDateModule` in your applications root module. We provide it at the component level
        // here, due to limitations of our example generation script.
        {
            provide: DateAdapter,
            useClass: MomentDateAdapter,
            deps: [MAT_DATE_LOCALE, MAT_MOMENT_DATE_ADAPTER_OPTIONS]
        },
        { provide: MAT_DATE_FORMATS, useValue: MAT_MOMENT_DATE_FORMATS },
    ],
    standalone: true,
    imports: [
        MatFormField,
        MatInput,
        ReactiveFormsModule,
        FormsModule,
        MatDatepickerInput,
        MatDatepickerToggle,
        MatSuffix,
        MatDatepicker,
        MatCheckbox,
        MatHint,
        AngularMultiSelectModule,
        MatButton,
    ],
})
export class LogSidebarComponent implements OnInit {
    private schemeService = inject(SchemeService);
    private sidebar = inject(SidebarService);

    /* Переменные для выпадающих */
    devItemGroups: DIG_DropdownData[] = [];
    devItems: DIG_DropdownData[] = [];
    devItemParams: DIG_DropdownData[] = [];
    statuses: DIG_DropdownData[] = [];

    readonly textEvents: {label: string, value: number}[] = [
        { label: 'Отладка', value: 0 }, // TODO: localization
        { label: 'Предупреждение', value: 1 },
        { label: 'Ошибка', value: 2 },
        { label: 'Инфо', value: 4 },
    ];

    digSelectSettings = {
        badgeShowLimit: 3,
        enableCheckAll: true,
        enableFilterSelectAll: false,
        enableSearchFilter: true,
        searchBy: ['label', 'folderName'],
        searchPlaceholderText: '',
        groupBy: 'folderName',
        labelKey: 'label',
        primaryKey: 'value',
        singleSelection: false,
    } as DropdownSettings;

    textEventsSettings = {
        badgeShowLimit: 4,
        enableCheckAll: true,
        enableFilterSelectAll: false,
        enableSearchFilter: false,
        labelKey: 'label',
        primaryKey: 'value',
        singleSelection: false,
    } as DropdownSettings;

    statusSettings = {
        ...this.textEventsSettings,
        groupBy: 'folderName',
    } as DropdownSettings;

    /* Переменные для работы со временем */
    date_from = new UntypedFormControl(moment());
    time_from = '00:00:00';
    date_to = new UntypedFormControl(moment());
    time_to = '23:59:59';

    /* Переменные для выбранных/введенных значений */

    selectedLogs: SelectedLogs = {
        event: true,
        mode: true,
        param: true,
        status: true,
        value: false,
    };

    selectedTextEvents: { label: string, value: number }[] = [];

    filter = '';
    case_sensitive = false;

    selectedGroupsId: DIG_DropdownData[] = [];
    selectedItemsId: DIG_DropdownData[] = [];
    selectedParamsId: DIG_DropdownData[] = [];
    selectedStatuses: DIG_DropdownData[] = [];

    ngOnInit(): void {
        this.sidebar.resetSidebar();

        this.devItemGroups = this.schemeService.scheme.section
            .reduce((prev: DIG_DropdownData[], curr) => {
                const groups = curr.groups.map(group => ({
                    folderName: curr.name,
                    label: group.title || group.type.title,
                    value: group.id,
                }));

                return prev.concat(groups);
            }, []);

        this.getDropdownDevItemsFromSections();
        this.getDropdownDevItemParamsFromSections();
        this.getStatusesForDropdown();

        this.fetchSelectedLogsFromLS();
        this.setupDatetimeRange();

        this.submit();
    }

    getStatusesForDropdown() {
        this.statuses = this.schemeService.scheme.dig_status_type.map((status) => ({
            label: status.text,
            value: status.id,
            folderName: this.schemeService.scheme.dig_status_category
                .find((cat) => status.category_id === cat.id).title,
        }));
    }

    getDropdownDevItemsFromSections() {
        const cb = (group: Device_Item_Group, section: string) => {
            let folderName = group.title || group.type.title;
            if (this.schemeService.scheme.section.length > 1) {
                folderName = `${section}->${folderName}`;
            }
            return group.items.map(item => ({
                label: item.name || item.type.title,
                value: item.id,
                folderName,
            }));
        }

        this.devItems = this.itemsForDropdownFromSections(cb);
    }

    getDropdownDevItemParamsFromSections() {
        const cb2 = (params: DIG_Param[], folderName: string): DIG_DropdownData[] => {
            return params.reduce((result: DIG_DropdownData[], param) => {
                result.push({
                    label: param.param.title,
                    value: param.id,
                    folderName,
                });

                if (param.childs) {
                    result = result.concat(cb2(param.childs, folderName));
                }

                return result;
            }, []);
        };

        const cb = (group: Device_Item_Group, section: string): DIG_DropdownData[] => {
            let folderName = group.title || group.type.title;
            if (this.schemeService.scheme.section.length > 1) {
                folderName = `${section}->${folderName}`;
            }

            return cb2(group.params, folderName);
        };

        this.devItemParams = this.itemsForDropdownFromSections(cb);
    }

    itemsForDropdownFromSections(
        callback: (group: Device_Item_Group, section: string) => DIG_DropdownData[]
    ): DIG_DropdownData[] {
        return this.schemeService.scheme.section
            .reduce((prev: DIG_DropdownData[], curr) => {
                const sectionName = this.schemeService.scheme.section.length > 1 ? `${curr.name}: ` : '';
                const devItems = curr.groups.reduce((items: DIG_DropdownData[], group) => {
                    const groupItems = callback(group, sectionName);

                    return items.concat(groupItems);
                }, []);

                return prev.concat(devItems);
            }, []);
    }

    submit() {
        this.storeSelectedLogsToLS();

        const ts_from = parseDate(this.date_from, this.time_from);
        const ts_to = parseDate(this.date_to, this.time_to);

        const data: LogsFilter = {
            ts_from,
            ts_to,
            selectedLogs: {...this.selectedLogs},
            selectedTextEvents: this.selectedTextEvents.map(i => i.value),
            filter: this.filter,
            case_sensitive: this.case_sensitive,
            selectedGroupsId: this.selectedGroupsId.map(g => g.value),
            selectedItemsId: this.selectedItemsId.map(g => g.value),
            selectedParamsId: this.selectedParamsId.map(g => g.value),
            selectedStatusesId: this.selectedStatuses.map(g => g.value),
        };

        this.sidebar.performActionToContent({
            type: 'params_changed',
            data,
        });
    }

    private fetchSelectedLogsFromLS() {
        const selectedLogs = localStorage.getItem('selectedLogs');
        try {
            if (selectedLogs) {
                Object.assign(this.selectedLogs, JSON.parse(selectedLogs));
            }
        } catch (e) {}
    }

    private storeSelectedLogsToLS() {
        localStorage.setItem('selectedLogs', JSON.stringify(this.selectedLogs))
    }

    private setupDatetimeRange() {
        const now = Date.now();
        const ts_from = now - 3 * 3600 * 1000; // 3600 -- hour in seconds, 1000 - ms per s
        const ts_to = new Date();
        ts_to.setHours(23, 59, 59);

        this.time_from = parseDateToDateAndTime(ts_from, this.date_from);
        this.time_to = parseDateToDateAndTime(ts_to.getTime(), this.date_to);
    }
}
