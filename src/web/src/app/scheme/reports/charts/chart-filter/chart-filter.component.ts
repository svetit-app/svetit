import { Component, IterableDiffer, IterableDiffers, OnDestroy, OnInit, inject } from '@angular/core';
import { UntypedFormControl, ReactiveFormsModule, FormsModule } from '@angular/forms';
import moment from 'moment';
import {BuiltChartParams, Chart_Info_Interface, Chart_Params, Chart_Type, ChartFilter, Select_Item_Iface} from '../chart-types';
import {
    Axis_Config,
    Axis_Params,
    Chart_Item,
    Chart_Item_Config,
    Chart_old,
    Device_Item,
    Device_Item_Group,
    DIG_Param,
    DIG_Param_Value_Type,
    Save_Algorithm,
    Saved_User_Chart,
    Section
} from '../../../scheme';
import {SchemeService} from '../../../scheme.service';
import {ColorPickerDialog, Hsl} from '../color-picker-dialog/color-picker-dialog';
import { DateAdapter, MAT_DATE_FORMATS, MAT_DATE_LOCALE, MatOption } from '@angular/material/core';
import {MAT_MOMENT_DATE_ADAPTER_OPTIONS, MAT_MOMENT_DATE_FORMATS, MomentDateAdapter} from '@angular/material-moment-adapter';
import {SidebarAction, SidebarService} from '../../../sidebar.service';
import {Subscription} from 'rxjs';
import {TranslateService} from '@ngx-translate/core';
import {MatDialog} from '@angular/material/dialog';
import { DecimalPipe } from '@angular/common';
import { MatIcon } from '@angular/material/icon';
import { MatSlideToggle } from '@angular/material/slide-toggle';
import { MatFormField, MatSuffix, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatDatepickerInput, MatDatepickerToggle, MatDatepicker } from '@angular/material/datepicker';
import { MatRadioGroup, MatRadioButton } from '@angular/material/radio';
import { MatSelect } from '@angular/material/select';
import { AngularMultiSelectModule } from 'angular2-multiselect-dropdown';
import { MatButton } from '@angular/material/button';

interface Chart_Item_Iface {
    id: number;
    hsl: Hsl;
    stepped: boolean;
}

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

@Component({
    selector: 'app-chart-filter',
    templateUrl: './chart-filter.component.html',
    styleUrls: ['./chart-filter.component.css'],
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
    imports: [MatIcon, ReactiveFormsModule, FormsModule, MatSlideToggle, MatFormField, MatInput, MatDatepickerInput, MatDatepickerToggle, MatSuffix, MatDatepicker, MatRadioGroup, MatRadioButton, MatLabel, MatSelect, MatOption, AngularMultiSelectModule, MatButton, DecimalPipe]
})
export class ChartFilterComponent implements OnInit, OnDestroy {
    private schemeService = inject(SchemeService);
    private sidebar = inject(SidebarService);
    private dateAdapter = inject<DateAdapter<any>>(DateAdapter);
    private translate = inject(TranslateService);
    private dialog = inject(MatDialog);
    private differs = inject(IterableDiffers);

    chartType = Chart_Type;
    params: ChartFilter;

    chart_filter: ChartFilter;
    charts: Chart_Info_Interface[];

    // ngModels
    charts_type: Chart_Type = Chart_Type.CT_USER;
    date_from = new UntypedFormControl(moment());
    time_from = '00:00:00';
    date_to = new UntypedFormControl(moment());
    time_to = '23:59:59';
    user_chart: Chart_old | Saved_User_Chart;
    itemList = [];
    selected_chart: Chart_Params;
    axes: Axis_Params[] = [];
    settings: any = {};
    user_charts: (Chart_old | Saved_User_Chart)[] = [];

    paramList: Select_Item_Iface[] = [];
    paramSettings: any = {};

    userChartSelect = false;

    private selectedItems_ = [];
    private paramSelected_: Select_Item_Iface[] = [];
    draggingDataset: boolean;
    private draggingDatasetTo:  Axis_Params;

    get selectedItems() {
        return this.selectedItems_;
    }

    get paramSelected() {
        return this.paramSelected_;
    }

    set selectedItems(v) {
        this.selectedItems_ = v;
        if (!this.selectedItemsDiffer && v) {
            this.selectedItemsDiffer = this.differs.find(v).create();
        }
    }

    set paramSelected(v) {
        this.paramSelected_ = v;
        if (!this.paramSelectedDiffer && v) {
            this.paramSelectedDiffer = this.differs.find(v).create();
        }
    }

    private selectedItemsDiffer: IterableDiffer<any>;
    private paramSelectedDiffer: IterableDiffer<any>;

    data_part_size = 100000;
    private sidebarActionBroadcast$: Subscription;
    private is_first_update = true;
    private is_user_charts_loaded = false;

    constructor() {
        this.dateAdapter.setLocale(this.translate.currentLang);
        this.sidebarActionBroadcast$ = this.sidebar.getSidebarActionBroadcast()
            .subscribe((action) => this.sidebarAction(action));
    }

    ngOnInit(): void {
        this.fetchUserCharts();
    }

    ngOnDestroy(): void {
        this.sidebarActionBroadcast$.unsubscribe();
        this.sidebar.resetSidebar();
        this.sidebar.resetContent();
    }

    chartFilterUpdated(chartFilter: ChartFilter) {
        this.charts_type = chartFilter.charts_type;
        this.data_part_size = chartFilter.data_part_size;
        this.user_chart = chartFilter.user_chart;

        this.time_from = parseDateToDateAndTime(chartFilter.timeFrom, this.date_from);
        this.time_to = parseDateToDateAndTime(chartFilter.timeTo, this.date_to);

        if (!this.user_charts.length && !this.charts_type) {
            this.charts_type = Chart_Type.CT_DIG_TYPE;
        }

        if (chartFilter.selectedItems && chartFilter.selectedItems.length > 0) {
            this.OnChartsType();

            this.selectedItems = chartFilter.selectedItems;
            this.paramSelected = chartFilter.paramSelected;

            this.onItemSelect(this.selectedItems[0]);
        }

        if (this.is_first_update && this.is_user_charts_loaded) {
            this.is_first_update = false;
            this.buildChart();
        }
    }

    OnChartsType(user_chart: Chart_old | Saved_User_Chart = undefined): void {
        if (user_chart) {
            this.user_chart = {...user_chart};
        } else {
            this.user_chart = {id: 0, name: ''} as Chart_old;
        }

        this.axes = [];
        this.selectedItems = [];
        this.settings = {
            text: '',
            selectAllText: this.translate.instant('SELECT_ALL'),
            // unSelectAllText: 'Снять все',
            classes: 'chart-type-data ctd-items',
            enableSearchFilter: true,
            labelKey: 'title',
            singleSelection: false,
            groupBy: '',
            clearAll: false,
        };

        this.paramSelected = [];
        this.paramSettings = {
            text: '',
            selectAllText: this.translate.instant('SELECT_ALL'),
            classes: 'chart-type-data custom-class',
            enableSearchFilter: true,
            labelKey: 'title',
            groupBy: 'category'
        };

        switch (this.charts_type) {
            case Chart_Type.CT_USER:
                this.itemList = this.user_charts;
                if (user_chart) {
                    this.selectedItems.push(user_chart);
                } else {
                    if (this.itemList.length) {
                        this.selectedItems.push(this.itemList[0]);
                    }
                }

                this.userChartSelect = true;
                this.paramList = null;

                break;
            case Chart_Type.CT_DIG_TYPE:
                this.userChartSelect = false;

                this.itemList = this.schemeService.scheme.dig_type;
                if (this.itemList.length) {
                    this.selectedItems.push(this.itemList[0]);
                    this.onItemSelect(this.selectedItems[0]);
                }
                this.settings.text = this.translate.instant('REPORTS.SELECT_GROUP_TYPE');
                this.settings.singleSelection = true;

                this.paramSettings.text = this.translate.instant('REPORTS.SELECT_PARAM_TYPE');
                break;
            case Chart_Type.CT_DEVICE_ITEM:
                this.userChartSelect = false;

                this.itemList = this.getDevItemList();
                this.settings.text = this.translate.instant('REPORTS.SELECT_ITEM');
                this.settings.groupBy = 'category';
                this.settings.clearAll = true;

                this.paramList = this.getParamList();
                this.paramSettings.text = this.translate.instant('REPORTS.SELECT_PARAM');
                break;
            default:
                break;
        }

        this.rebuild();
    }

    onItemSelect(item: any): void {
        if (this.charts_type === Chart_Type.CT_DIG_TYPE) {
            const accepted_param_type_ids = this.schemeService.scheme.dig_param_type
                .filter(param_type => param_type.group_type_id === item.id)
                .map(param_type => param_type.id);

            this.paramSelected = [];
            this.paramList = this.getParamTypeList().filter((param_type: Select_Item_Iface) => {
                return accepted_param_type_ids.includes(param_type.id);
            });
        }

        if (this.charts_type === Chart_Type.CT_USER) {
            this.selectUserChart(item);
        }
    }

    rebuild() {
        if (!this.selectedItemsDiffer.diff(this.selectedItems) && !this.paramSelectedDiffer.diff(this.paramSelected)) {
            return;
        }

        this.selected_chart = null;
        switch (this.charts_type) {
            case Chart_Type.CT_USER:
                this.initDeviceUserDatasets();
                break;
            case Chart_Type.CT_DIG_TYPE:
                this.initDigTypeDatasets();
                break;
            case Chart_Type.CT_DEVICE_ITEM:
                this.initDeviceItemDatasets();
                break;
        }
    }

    initDeviceItemDatasetsImpl(dev_items: Chart_Item_Iface[], params: Chart_Item_Iface[], axe: Axis_Params): void {
        const sections = this.schemeService.scheme.section;

        const filteredDevItems = dev_items.filter((dev_item) => {
            return !this.axes.find((axis) => !!axis.datasets.find(item => item.item_id === dev_item.id));
        });

        for (const sct of sections) {
            for (const group of sct.groups) {
                for (const item of group.items) {
                    for (const s_item of filteredDevItems) {
                        if (s_item.id == item.id) {
                            ChartFilterComponent.pushDatasetToAxe(axe, item, s_item.hsl, s_item.stepped,false);
                            // ChartFilterComponent.pushToDatasetParams(dataset_params, item);
                            break;
                        }
                    }
                }

                this.addParam2Axe(axe, group.params, params);
            }
        }

        this.selected_chart = {
            axes: this.axes,
            name: this.translate.instant('REPORTS.CHARTS_ELEMENTS'),
        };
    }

    initDeviceUserDatasets(): void {
        const user_chart = this.selectedItems[0] as Chart_old | Saved_User_Chart;
        if (!user_chart) return;

        if ((<Chart_old>user_chart).items) {
            this.setupUserChart(<Chart_old>user_chart);
        } else {
            this.setupUserChartNew(<Saved_User_Chart>user_chart);
        }

        this.selected_chart = {
            name: user_chart.name,
            axes: this.axes,
        };
    }

    initDeviceItemDatasets(): void {
        const items = this.selectedItems.map(it => { return {id: it.id, hsl: null, stepped: null}; });
        const params = this.paramSelected.map(it => { return {id: it.id, hsl: null, stepped: null}; });
        this.initDeviceItemDatasetsImpl(items, params, this.getCurrentAxis());
    }

    initDigTypeDatasets(): void {
        const params = this.get_dig_param_ids(this.paramSelected);
        const sections = this.schemeService.scheme.section;

        const [itemAxis, paramAxis] = this.getDigAxes();

        for (const sct of sections) {
            for (const group of sct.groups) {
                if (group.type_id === this.selectedItems[0].id) {
                    for (const item of group.items) {
                        if (item.type.save_algorithm > Save_Algorithm.SA_OFF) {
                            ChartFilterComponent.pushDatasetToAxe(itemAxis, item, null, null,false);
                        }
                    }

                    this.addParam2Axe(paramAxis, group.params, params);

                    this.selected_chart = {
                        name: sct.name,
                        axes: this.axes,
                    };
                    break;
                }
            }
        }
    }

    get_dig_param_ids(param_types: any[]): Chart_Item_Iface[]
    {
        const get_param_id = (param_type_id: number): number =>
        {
            const find_param = (params: DIG_Param[], type_id: number) =>
            {
                if (params)
                    for (const param of params)
                    {
                        if (param.param_id === type_id)
                            return param.id;

                        const param_id = find_param(param.childs, type_id);
                        if (param_id)
                            return param_id;
                    }
                return null;
            };

            for (const sct of this.schemeService.scheme.section)
            {
                for (const group of sct.groups)
                {
                    const param_id = find_param(group.params, param_type_id);
                    if (param_id)
                        return param_id;
                }
            }
            console.warn('Search param type id: ' + param_type_id + ' failed', this.schemeService.scheme.section);
            return null;
        };

        let res = [];
        for (const param_type of param_types)
        {
            const dig_param_id = get_param_id(param_type.id);
            if (dig_param_id)
                res.push({id: dig_param_id, hsl: null });
        }

        return res;
    }

    addParam2Axe(axe:  Axis_Params, params: DIG_Param[], selected: Chart_Item_Iface[]): void {
        const filtered = selected.filter((param) => {
            return !this.axes.find((axis) => !!axis.datasets.find(item => item.param_id === param.id));
        });
        for (const param of params) {
            for (const s_pt of filtered) {
                if (s_pt.id === param.id) {
                    ChartFilterComponent.pushDatasetToAxe(axe, param, s_pt.hsl, s_pt.stepped,true);
                    // ChartFilterComponent.pushToDatasetParams(datasets, param, true);
                    break;
                }
            }

            if (param.childs)
                this.addParam2Axe(axe, param.childs, selected);
        }
    }

    getDevItemList(): Select_Item_Iface[] {
        let devItemList = [];
        for (const sct of this.schemeService.scheme.section) {
            for (const group of sct.groups) {
                const po = this.getPrefixObj(sct, group);

                for (const item of group.items) {
                    const title = po.prefix + (item.name || item.type.title);
                    devItemList.push({id: item.id, title, category: po.category});
                }
            }
        }

        return devItemList;
    }

    getParamList(): Select_Item_Iface[] {
        let paramList: Select_Item_Iface[] = [];

        let add_param = (p: DIG_Param, category: string = '') => {
            if (p.childs && p.childs.length) {
                const new_category = category + ' - ' + p.param.title;
                for (const p1 of p.childs) {
                    add_param(p1, new_category);
                }
            } else {
                paramList.push({id: p.id, title: p.param.title, category});
            }
        };

        for (const sct of this.schemeService.scheme.section) {
            for (const group of sct.groups) {
                const po = this.getPrefixObj(sct, group);

                for (const prm of group.params) {
                    add_param(prm, po.category + po.prefix);
                }
            }
        }

        return paramList;
    }

    getParamTypeList(): Select_Item_Iface[] {
        const paramList: Select_Item_Iface[] = [];
        const param_types = this.schemeService.scheme.dig_param_type;
        for (const pt of param_types) {
            if (pt.value_type >= DIG_Param_Value_Type.VT_RANGE
                || pt.value_type <= DIG_Param_Value_Type.VT_UNKNOWN) {
                continue;
            }

            let category = '?';
            for (const dig_type of this.schemeService.scheme.dig_type) {
                if (dig_type.id === pt.group_type_id) {
                    category = dig_type.title;
                    break;
                }
            }

            let title = pt.title;
            if (pt.parent_id) {
                for (const p_pt of param_types) {
                    if (p_pt.id === pt.parent_id) {
                        title = p_pt.title + ' - ' + title;
                        break;
                    }
                }
            }

            paramList.push({id: pt.id, title, category});
        }

        return paramList;
    }

    getPrefixObj(sct: Section, group: Device_Item_Group): any {
        let prefix = '';
        let category;
        if (this.schemeService.scheme.section.length > 1) {
            category = sct.name;
            prefix = (group.title || group.type.title) + ' - ';
        } else {
            category = group.title || group.type.title;
        }
        return {prefix, category};
    }

    edit_user_chart(): void {
        const user_chart = this.selectedItems[0];

        this.charts_type = Chart_Type.CT_DEVICE_ITEM;
        this.OnChartsType(user_chart);

        this.selected_chart = null;
        this.axes = [];
        if (user_chart.items) {
            this.selectedItems = this.itemList.filter(item => {
                for (const it of user_chart.items) {
                    if (item.id === it.item_id) {
                        return true;
                    }
                }
                return false;
            });

            this.paramSelected = this.paramList.filter(item => {
                for (const it of user_chart.items) {
                    if (item.id === it.param_id) {
                        return true;
                    }
                }
                return false;
            });

            this.setupUserChart(user_chart, true);
        } else {
            const { items, params } = this.setupUserChartNew(user_chart, true);
            this.selectedItems = this.itemList.filter(item => {
                for (const it of items) {
                    if (item.id === it.id) {
                        return true;
                    }
                }
                return false;
            });

            this.paramSelected = this.paramList.filter(item => {
                for (const it of params) {
                    if (item.id === it.id) {
                        return true;
                    }
                }
                return false;
            });
        }
        this.selected_chart = {
            axes: this.axes,
            name: this.translate.instant('REPORTS.CHARTS_ELEMENTS'),
        };
        this.buildChart();
    }

    save_user_chart(): void {
        if (!this.user_chart.name.length) {
            return;
        }

        if (!this.selectedItems.length && !this.paramSelected.length) {
            return;
        }

        const axes = this.axes.map((axe) => {
            const datasets: Chart_Item_Config[] = axe.datasets
                .map(({ item_id, param_id, isParam, extra: { stepped, color, hidden }}) => ({
                    item_id, param_id, isParam,
                    extra: {
                        hidden,
                        stepped,
                        color,
                    },
                }));

            const { id, isRight, display, from, to, order, displayGrid } = axe;

            return {
                id,
                datasets,
                isRight,
                display,
                displayGrid,
                from,
                to,
                order,
            };
        });

        const userChart = new Saved_User_Chart();
        userChart.id = this.user_chart.id;
        userChart.name = this.user_chart.name;
        userChart.axes = axes;

        this.schemeService.save_chart(userChart)
            .subscribe((new_chart) => {
                let needAdd = true;
                const foundChartIdx = this.user_charts.findIndex(chart => chart.id === new_chart.id);
                if (foundChartIdx >= 0) {
                    const foundChart = this.user_charts[foundChartIdx];
                    if ((<Saved_User_Chart>foundChart).axes) {
                        foundChart.name = new_chart.name;
                        (<Saved_User_Chart>foundChart).axes = new_chart.axes;
                        needAdd = false;
                    } else {
                        this.user_charts.splice(foundChartIdx, 1);
                    }
                }

                if (needAdd) { // make chart instanceof Saved_User_Chart
                    const chart = new Saved_User_Chart();
                    chart.id = new_chart.id;
                    chart.name = new_chart.name;
                    chart.axes = new_chart.axes;
                    this.user_charts.push(chart);
                }
            });
    }

    del_user_chart(): void {
        const user_chart = this.selectedItems[0];

        this.schemeService.del_chart(user_chart).subscribe(is_removed => {
            if (!is_removed) {
                return;
            }

            for (const chart_i in this.user_charts) {
                if (this.user_charts[chart_i].id === user_chart.id) {
                    this.user_charts.splice(parseInt(chart_i), 1);
                    break;
                }
            }

            this.selectedItems = [];
            if (this.user_charts.length) {
                this.selectUserChart(this.user_charts[0]);
            }
        });
    }

    buildChart() {
        this.sidebar.performActionToContent({
            type: 'params_change',
            data: {
                timeFrom: parseDate(this.date_from, this.time_from),
                timeTo: parseDate(this.date_to, this.time_to),
                selected_chart: this.selected_chart,
                user_chart: this.user_chart,
                user_charts: this.user_charts,
                charts_type: this.charts_type,
                data_part_size: this.data_part_size,
            },
        });
    }

    private sidebarAction(action: SidebarAction<any>) {
        if (action.type === 'chart_filter') {
            this.chartFilterUpdated(action.data);
        }

        if (action.type === 'charts') {
            this.charts = action.data.charts;

            if (!this.chart_filter) {
                this.chartFilterUpdated(action.data.chart_filter);
            }
        }

        if (action.type === 'chart_axes') {
            const builtChartParams = <BuiltChartParams>action.data;
            builtChartParams.axes.forEach((axe) => {
                const foundAxis = this.axes.find(a => a.id === axe.id);
                if (foundAxis) {
                    foundAxis.from = axe.from;
                    foundAxis.to = axe.to;
                    foundAxis.display = axe.display;

                    builtChartParams.datasets.forEach((dataset) => {
                        const isParam = !!dataset.param;
                        const foundDataset = foundAxis.datasets.find((ds) => {
                            if (ds.isParam !== isParam) return false;
                            if (isParam) {
                                return ds.param_id === dataset.param.id;
                            } else {
                                return ds.item_id === dataset.dev_item.id;
                            }
                        });

                        if (foundDataset) {
                            foundDataset.extra.stepped = dataset.stepped as boolean;
                        }
                    });
                }
            });
        }
    }

    openColorPicker(dataset: Chart_Item): void {
        const dialogRef = this.dialog.open(ColorPickerDialog, {
            width: '450px',
            data: { dataset },
        });

        dialogRef.afterClosed().subscribe(hsl => {
            if (hsl !== undefined && hsl !== null)
            {
                dataset.extra.color = hsl;
                this.dataset_legend_updated(dataset);
            }
        });
    }

    toggleDatasetVisibility(dataset: Chart_Item): void {
        dataset.extra.hidden = !dataset.extra.hidden;
        this.dataset_legend_updated(dataset);
    }

    private static getColorByIndex(index: number, label: string): Hsl
    {
        switch (index)
        {
            case 0: return { h: 0, s: 100, l: 35 }; // red
            case 1: return { h: 120, s: 100, l: 35 }; // green
            case 2: return { h: 240, s: 100, l: 35 }; // blue
            case 3: return { h: 60, s: 100, l: 35 }; // yellow
            case 4: return { h: 180, s: 100, l: 35 }; // cyan
            case 5: return { h: 300, s: 100, l: 35 }; // magenta
            case 6: return { h: 30, s: 100, l: 35 }; // brown
            case 7: return { h: 90, s: 100, l: 35 }; // green
            case 8: return { h: 150, s: 100, l: 35 }; //
            case 9: return { h: 210, s: 100, l: 35 }; //
            case 10: return { h: 270, s: 100, l: 35 }; //
            case 11: return { h: 330, s: 100, l: 35 }; //
            default:
                return { h: this.hashCode(label), s: 95, l: 35 } as Hsl;
        }
    }

    private static hashCode(str: string): number { // java String#hashCode
        let hash = 0;
        for (let i = 0; i < str.length; i++)
            hash = str.charCodeAt(i) + ((hash << 5) - hash);
        return hash;
    }

    private dataset_legend_updated(dataset: Chart_Item) {
        const hsl = dataset.extra.color;
        dataset.extra.displayColor = `hsl(${hsl.h}, ${hsl.s}%, ${hsl.l}%)`;

        this.sidebar.performActionToContent({
            type: 'legend_updated',
            data: dataset,
        });
    }

    toggleLegendModal(axe: Axis_Config) {
        axe.showModal = !axe.showModal;

        if (axe.showModal) {
            return;
        }

        // Process inputted values if modal closed
        if (axe) {
            if (axe.from && typeof axe.from === 'string') {
                axe.from = parseFloat(axe.from);
            }

            if (axe.to && typeof axe.to === 'string') {
                axe.to = parseFloat(axe.to);
            }

            if (axe.order && typeof axe.order === 'string') {
                axe.order = parseInt(axe.order, 10);
            }
        }
    }

    private fetchUserCharts() {
        this.schemeService.get_charts().subscribe(charts => {
            this.user_charts = charts;
            this.is_user_charts_loaded = true;

            if (this.is_first_update) {
                this.is_first_update = false;

                if (this.user_charts.length > 0) {
                    this.selectUserChart(this.user_charts[0]);
                } else {
                    this.buildChart();
                }
            }
        });
    }

    private selectUserChart(chart: Chart_old | Saved_User_Chart) {
        this.charts_type = Chart_Type.CT_USER;

        this.selectedItems = [];
        this.paramSelected = [];

        this.OnChartsType(chart);
        this.buildChart();
    }

    addAxis() {
        const id = String.fromCharCode('A'.charCodeAt(0) + this.axes.length);
        let maxOrder;

        if (this.axes.length === 0) {
            maxOrder = 0;
        } else {
            maxOrder = this.axes
                .map(axe => +axe.order)
                .reduce((prev, curr) => prev > curr ? prev : curr);
        }

        const axe: Axis_Params = {
            id,
            isRight: false,
            display: 'auto',
            displayGrid: true,
            from: -50,
            to: 50,
            order: maxOrder,
            showModal: false,
            datasets: [],
        };

        this.axes.push(axe);
        return axe;
    }

    private findAxisById(id: string): Axis_Params {
        return this.axes.find((axis) => axis.id === id);
    }

    private getDigAxes(): Axis_Params[] {
        if (this.charts_type === Chart_Type.CT_DIG_TYPE) {
            if (this.axes.length !== 2) {
                this.axes = [];

                this.addAxis(); // for item default
                this.addAxis(); // for param

                const [, param] = this.axes;
                param.from = -1;
                param.to = 2;
                param.isRight = true;
            }

            return this.axes;
        }

        throw new Error('Method allowed only when charts_type is CT_DIG_TYPE');
    }

    private getCurrentAxis() {
        if (this.axes.length === 0) {
            this.addAxis();
        }

        return this.axes[this.axes.length - 1];
    }

    private static pushDatasetToAxe(
        axe:  Axis_Params,
        item: Device_Item | DIG_Param,
        color: Hsl,
        stepped: boolean,
        isParam: boolean
    ) {
        if (!color) {
            const title = (<Device_Item>item).type?.title || (<DIG_Param>item).param?.title;
            color = ChartFilterComponent.getColorByIndex(axe.datasets.length, title);
        }

        axe.datasets.push({
            item_id: !isParam ? item.id : null,
            param_id: isParam ? item.id : null,
            item,
            isParam,
            extra: {
                color,
                displayColor: `hsl(${color.h}, ${color.s}%, ${color.l}%)`,
                title: isParam ? `⚙️ ${(<DIG_Param>item).param.title}` : (<Device_Item>item).type.title,
                stepped,
                hidden: false,
            },
        });
    }

    datasetDrag($event: DragEvent) {
        $event.preventDefault();

        this.draggingDataset = true;
        $event.dataTransfer.dropEffect = 'move';
    }

    datasetDragEnd($event: DragEvent, ds: Chart_Item, from: Axis_Config & {datasets: Chart_Item[]}) {
        $event.preventDefault();

        this.draggingDataset = false;
        if (this.draggingDatasetTo) {
            const idx = from.datasets.indexOf(ds);
            from.datasets.splice(idx, 1);

            this.draggingDatasetTo.datasets.push(ds);
            this.draggingDatasetTo = null;
        }
    }

    dragover($event: DragEvent, axe:  Axis_Params) {
        $event.preventDefault();

        this.draggingDatasetTo = axe;
        axe.allowDragOver = true;
    }

    dragleave($event: DragEvent, axe:  Axis_Params) {
        $event.preventDefault();

        axe.allowDragOver = false;
        this.draggingDatasetTo = null;
    }

    private setupUserChart(user_chart: Chart_old, displayAuto: boolean = false) {
        let itemsPerAxis: Record<string, Chart_Item_Iface[]> = {};
        let paramsPerAxis: Record<string, Chart_Item_Iface[]> = {};

        for (const it of user_chart.items) {
            const scale = it.extra.axis_params;
            let axis = this.axes.find(axis => axis.from === scale.from
                && axis.to === scale.to
                && axis.isRight === scale.isRight
                && axis.display === scale.display
            );

            if (!axis) {
                if (scale.from || scale.to) {
                    // create and assign new axis if valid params
                    axis = this.addAxis();
                    axis.isRight = scale.isRight;
                    axis.from = scale.from;
                    axis.to = scale.to;
                    axis.display = scale.display;
                } else {
                    // assign previous if one or more exist
                    if (this.axes.length > 0) {
                        axis = this.axes[this.axes.length - 1];
                    } else {
                        axis = this.addAxis();
                    }
                }
            }

            if (displayAuto) {
                axis.display = 'auto';
            }

            if (it.item_id) {
                itemsPerAxis[axis.id] ??= [];
                itemsPerAxis[axis.id].push({id: it.item_id, stepped: it.extra.axis_params.stepped || null, hsl: ColorPickerDialog.rgbhex2hsl(it.extra.color)});
            } else {
                paramsPerAxis[axis.id] ??= [];
                paramsPerAxis[axis.id].push({id: it.param_id, stepped: it.extra.axis_params.stepped || null, hsl: ColorPickerDialog.rgbhex2hsl(it.extra.color)});
            }
        }

        let keys = Object.keys(itemsPerAxis);
        const filteredParamKeys = Object.keys(paramsPerAxis).filter(paramKey => keys.indexOf(paramKey) === -1);
        keys = keys.concat(filteredParamKeys);

        keys.forEach((key) => {
                const items = itemsPerAxis[key] || [];
                const params = paramsPerAxis[key] || [];
                const axis = this.findAxisById(key);

                this.initDeviceItemDatasetsImpl(items, params, axis);
            });
    }

    private setupUserChartNew(userChart: Saved_User_Chart, displayAuto: boolean = false) {
        const { axes } = userChart;
        let itemsFromAllAxes: Chart_Item_Iface[] = [];
        let paramsFromAllAxes: Chart_Item_Iface[] = [];
        this.axes = axes.map((axe) => {
            const axisParam = {
                ...axe,
                datasets: [],
            };

            if (displayAuto) {
                axisParam.display = 'auto';
            }

            const items: Chart_Item_Iface[] = [];
            const params: Chart_Item_Iface[] = [];
            axe.datasets.forEach((ds) => {
                if (ds.isParam) {
                    params.push({id: ds.param_id, hsl: ds.extra.color, stepped: ds.extra.stepped});
                } else {
                    items.push({id: ds.item_id, hsl: ds.extra.color, stepped: ds.extra.stepped});
                }
            });

            this.initDeviceItemDatasetsImpl(items, params, axisParam);
            itemsFromAllAxes = itemsFromAllAxes.concat(items);
            paramsFromAllAxes = paramsFromAllAxes.concat(params);

            return axisParam;
        });

        return { items: itemsFromAllAxes, params: paramsFromAllAxes };
    }
}
