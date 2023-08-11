import {Chart_old, Axis_Config, Device_Item, DIG_Param, Chart_Item, Axis_Params} from '../../scheme';
import {Hsl} from './color-picker-dialog/color-picker-dialog';
import * as ChartJs from 'chart.js';
import Chart, {ChartOptions} from 'chart.js';

export enum Chart_Type {
    CT_UNKNOWN,
    CT_USER,
    CT_DIG_TYPE,
    CT_DEVICE_ITEM,
}

export interface Select_Item_Iface {
    id: number;
    title: string;
    category: string;
}

export interface Chart_Info_Interface {
    name: string;
    charts_type: Chart_Type;
    data: {
        datasets: (ChartJs.ChartDataset<'line'> & { dev_item?: Device_Item, param?: DIG_Param } & any)[];
    };

    axes?: ChartOptions<'line'>['scales'];
}

export interface TimeFilter
{
    timeFrom: number;
    timeTo: number;
}

export interface Chart_Params {
    name: string;
    axes: Axis_Params[];
}

export interface ChartFilter extends TimeFilter {
    user_chart: Chart_old;
    user_charts: Chart_old[];

    selected_chart?: Chart_Params;

    charts_type: Chart_Type;
    data_part_size: number;

    paramSelected?: any[];
    selectedItems?: any[];
}

export interface ZoomInfo extends TimeFilter {
    isZoom: boolean;
}

export interface BuiltChartParams {
    axes: Axis_Config[];
    datasets: (Chart.ChartDataset<'line'> & { dev_item?: Device_Item, param?: DIG_Param })[];
}

