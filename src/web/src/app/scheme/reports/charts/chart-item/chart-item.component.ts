import { AfterViewInit, ChangeDetectorRef, Component, DoCheck, ElementRef, EventEmitter, Input, KeyValueChanges, KeyValueDiffer, KeyValueDiffers, NgZone, OnChanges, OnInit, Output, SimpleChanges, ViewChild, inject } from '@angular/core';

import {MatSnackBar} from '@angular/material/snack-bar';

import {Paginator_Chart_Value, SchemeService} from '../../../scheme.service';
import {Scheme_Group_Member} from '../../../../user';
import {Hsl} from '../color-picker-dialog/color-picker-dialog';
import {BuiltChartParams, Chart_Info_Interface, Chart_Type, ZoomInfo} from '../chart-types';
import {LoadingProgressbar} from '../../../loading-progressbar/loading.progressbar';
import * as Chart from 'chart.js';

import zoomPlugin from 'chartjs-plugin-zoom';
import 'chartjs-adapter-moment';
import {Axis_Config} from '../../../scheme';
import {ChartOptions, LinearScale, TooltipItem} from 'chart.js';
import {ScaleWithLegendBox} from './scale-with-legend-box';
import { MatProgressBar } from '@angular/material/progress-bar';

Chart.Chart.register(
    Chart.TimeScale, Chart.LinearScale, Chart.LineController,
    Chart.PointElement, Chart.LineElement, zoomPlugin, ScaleWithLegendBox,
    Chart.Tooltip,
);

@Component({
    selector: 'app-chart-item',
    templateUrl: './chart-item.component.html',
    styleUrls: ['./chart-item.component.css'],
    standalone: true,
    imports: [MatProgressBar]
})
export class ChartItemComponent extends LoadingProgressbar implements OnInit, OnChanges, AfterViewInit, DoCheck {
    private schemeService = inject(SchemeService);
    private differs = inject(KeyValueDiffers);
    private zone = inject(NgZone);

    private readonly defaultYAxes_: any = {
        A: {
            id: 'A',
            type: 'LinearWithLegend',
            position: 'left',
            display: 'auto',
            title: {
                display: true,
                text: `      A      `,
                align: 'start',
                padding: { top: 5, bottom: -9 },
            },
            bounds: 'ticks',
            beginAtZero: false,
            offset: false,
            min: -50,
            max: 50,
            grid: {
                display: true,
                drawTicks: false,
            },
            ticks: {
                backdropPadding: 0,
                padding: 2,
                callback: val => Math.round(val),
            },
        },
        B: {
            id: 'B',
            type: 'LinearWithLegend',
            position: 'right',
            display: 'auto',
            title: {
                display: true,
                text: `      B      `,
                align: 'start',
                padding: { top: 5, bottom: -9 },
            },
            bounds: 'ticks',
            beginAtZero: false,
            offset: false,
            min: -50,
            max: 50,
            grid: {
                display: true,
                drawTicks: false,
            },
            ticks: {
                max: 2,
                min: -1,
                stepSize: 1,
                suggestedMin: 0,
                suggestedMax: 1,
                padding: 2,
                callback: val => Math.round(val),
            },
        },
    };

    private _chartInfo: Chart_Info_Interface;
    private _differ: KeyValueDiffer<number, any>;
    private _datasetsDiffers: { [key: string]: KeyValueDiffer<any, any> } = {};

    private readonly leftYAxisLsKey: string = 'scale-a-params';
    private readonly rightYAxisLsKey: string = 'scale-b-params';

    @ViewChild('chart_container', { read: ElementRef }) chartContainer: ElementRef<HTMLCanvasElement>;

    get chartInfo(): Chart_Info_Interface {
        return this._chartInfo;
    }

    @Input() set chartInfo(v: Chart_Info_Interface) {
        this._chartInfo = v;
        if (!this._differ && v.data?.datasets) {
            this._differ = this.differs.find(v.data.datasets).create();
        }
    }

    @Input() viewportMin: number;
    @Input() viewportMax: number;
    @Input() yAxes: ChartOptions<'line'>['scales'];

    chart: Chart.Chart<'line'>;
    @Output() rangeChange: EventEmitter<ZoomInfo> = new EventEmitter();
    @Output() axeChange: EventEmitter<BuiltChartParams> = new EventEmitter();
    @Output() built: EventEmitter<BuiltChartParams> = new EventEmitter();

    update(): void {
        if (!this.chart) return;

        this.zone.run(() => this.chart.update());

        const axes = this.getAxes();
        this.built.emit({
            axes,
            datasets: this.chart.data.datasets,
        });
    }

    chartOptions = {
        type: "line",
        data: {
            datasets: [],
        },
        options: {
            elements: {
                point: { radius: 0 },
                line: { tension: 0, borderWidth: 1 }
            },
            animation: false,
            resizeDelay: 100,
            responsive: true,
            maintainAspectRatio: true,
            legend: { display: false },
            interaction: {
                intersect: false,
                mode: 'nearest',
            },
            hover: {
                mode: 'nearest',
                intersect: false,
            },
            scales: {
                x: {
                    offset: false,
                    stacked: true,
                    type: 'time',
                    time: {
                        tooltipFormat: 'DD MMMM YYYY HH:mm:ss',
                        displayFormats: {
                            millisecond: 'HH:mm:ss.SSS',
                            second: 'HH:mm:ss',
                            minute: 'HH:mm',
                            hour: 'HH:mm',
                            day: 'DD MMM',
                        },
                    },
                    ticks: {
                        major: {
                            enabled: true,
                            fontStyle: 'bold',
                            fontColor: 'rgb(54, 143, 3)'
                        },
                        sampleSize: 10,
                        maxRotation: 30,
                        minRotation: 30,
                        min: undefined,
                        max: undefined,
                    },
                    afterFit: (scale) => {
                        scale.height = 40;
                    }
                },
                ...this.defaultYAxes_,
            },
            plugins: {
                zoom: {
                    pan: {
                        enabled: true,
                        mode: 'xy',
                        overScaleMode: 'y',
                        rangeMax: {x: new Date()}, // TODO: update this sometimes
                        onPanComplete: chart => this.onZoom(chart, false)
                    },
                    zoom: {
                        wheel: {
                            enabled: true,
                        },
                        mode: 'xy',
                        overScaleMode: 'y',
                        onZoomComplete: chart => this.onZoom(chart, true)
                    }
                },
                tooltip: {
                    display: true,
                    callbacks: {
                        label: this.onLabel(),
                    },
                },
            },
        },
    } as Chart.ChartConfiguration<'line'>;

    @Input() members: Scheme_Group_Member[];

    constructor() {
        const snackBar = inject(MatSnackBar);
        const changeDetectorRef = inject(ChangeDetectorRef);

        super(snackBar, changeDetectorRef);
    }

    ngOnInit(): void {
        if (this.chartInfo.charts_type === Chart_Type.CT_DIG_TYPE) {
            this.setupYAxisScale(this.leftYAxisLsKey, 'A');
            this.setupYAxisScale(this.rightYAxisLsKey, 'B');
        } else {
            Object.keys(this.yAxes || this.defaultYAxes_)
                .forEach((key) => {
                    const left = this.chartOptions.options.scales[key];
                    const right = (this.yAxes || this.defaultYAxes_)[key];
                    if (left && right) {
                        Object.assign(
                            this.chartOptions.options.scales[key],
                            (this.yAxes || this.defaultYAxes_)[key],
                        );
                    } else if (right) {
                        this.chartOptions.options.scales[key] = right;
                    }
                });
        }
    }

    ngAfterViewInit() {
        this.chartOptions.data = this.chartInfo.data;
        this.chart = new Chart.Chart<'line'>(this.chartContainer.nativeElement.getContext('2d'), this.chartOptions);
    }

    ngOnChanges(changes: SimpleChanges) {
        if (changes.viewportMin || changes.viewportMax) {
            if (changes.viewportMin.currentValue)
                this.chartOptions.options.scales.x.min = changes.viewportMin.currentValue;

            if (changes.viewportMax.currentValue)
                this.chartOptions.options.scales.x.max = changes.viewportMax.currentValue;
        }
    }

    ngDoCheck(): void {
        let apply = false;

        if (this._datasetsDiffers) {
            Object.keys(this._datasetsDiffers).forEach((key) => {
                const differ = this._datasetsDiffers[key];
                const changes = differ.diff(this.chartInfo.data.datasets[key]);

                if (changes) {
                    apply = true;
                }
            });
        }

        if (this._differ) {
            const changes = this._differ.diff(this.chartInfo.data.datasets);
            if (changes) {
                changes.forEachAddedItem((r) => {
                    this._datasetsDiffers[r.key] = this.differs.find(r.currentValue).create();
                    apply = true;
                });
            }
        }

        if (apply) {
            this.applyDatasetChanges();
        }
    }

    static getY(log: any, is_stepped: boolean): any {
        let y = log.value;
        if (y == undefined || y == null)
            return null;

        // if (log.value == null || /^(\-|\+)?([0-9]+|Infinity)$/.test(log.value))
        if (typeof y !== 'string')
            return y;
        if (/^(\-|\+)?([0-9\.]+|Infinity)$/.test(y))
            return parseFloat(y);

        if (is_stepped)
        {
            // TODO: Remove it
            if (y === 'Норма' || y === 'Открыто')
                return 1;
            else if (y === 'Низкий' || y === 'Закрыто')
                return 0;
        }

        console.log('Value dropped', log);
        return null;
    }

    addData(dataPack: Paginator_Chart_Value, data_param_name: string, additional = false): void
    {
        if (!additional) {
            for (const dataset of this.chartInfo.data.datasets) {
                if (dataset[data_param_name]) {
                    dataset.data.splice(0, dataset.data.length);
                }
            }
        }

        const findDataset = (data_param_name: string, id: number) =>
        {
            for (const dataset of this.chartInfo.data.datasets)
                if (dataset[data_param_name] && dataset[data_param_name].id == id)
                    return dataset;
            return null;
        };

        for (const log of dataPack.results)
        {
            const dataset = findDataset(data_param_name, log.item_id);
            if (!dataset)
                continue;

            for (const log_item of log.data)
            {
                const y = ChartItemComponent.getY(log_item, dataset.steppedLine);
                if (y === undefined)
                    continue;

                const x = new Date(log_item.time);
                let data = {x, y};
                if (log_item.user_id)
                {
                    if (!dataset.usered_data)
                        dataset.usered_data = {};
                    dataset.usered_data[x.getTime()] = log_item.user_id;
                }
                if (additional) {
                    const idx = dataset.data.findIndex(v => v.x > x);
                    if (idx < 0 || idx >= dataset.data.length) {
                        dataset.data.push(data);
                    } else {
                        dataset.data.splice(idx, 0, data);
                    }
                } else {
                    dataset.data.push(data);
                }
            }
        }

        this.update();
    }

    addDevItemValues(logs: Paginator_Chart_Value, additional = false): void
    {
        this.addData(logs, 'dev_item', additional);
    }

    addParamValues(params: Paginator_Chart_Value, additional = false): void
    {
        this.addData(params, 'param', additional);
    }

    setDataColor(dataset: any, hsl: Hsl): void
    {
        const hslStr = `${hsl.h}, ${hsl.s}%, ${hsl.l}%`;
        dataset.borderColor = `hsl(${hslStr}`;
        dataset.backgroundColor = `hsla(${hslStr},0.5)`;
        dataset.pointBorderColor = `hsla(${hslStr},0.7)`;
        dataset.pointBackgroundColor = `hsla(${hslStr},0.5)`;
    }

    onZoom(chart: any, isZoom: boolean): void {
        const xAxis = chart.chart.scales['x'];
        this.rangeChange.emit({timeFrom: Math.floor(xAxis.min), timeTo: Math.floor(xAxis.max), isZoom});

        if (this.chartInfo.charts_type === Chart_Type.CT_DIG_TYPE) {
            this.storeScaleParamsToLocalStorage();
        }

        const axes = this.getAxes();

        this.axeChange.emit({
            axes,
            datasets: this.chart.data.datasets,
        });
    }

    onLabel() {
        const members = this.members;
        return (item: TooltipItem<'line'>): string => {
            const dataset = item.dataset;
            const value = (<Chart.ScatterDataPoint>item.raw).y;
            let text: string = value.toString();

            const dev_item = dataset['dev_item'];
            if (dev_item) {
                const value_view = dev_item.type.views?.find(vv => vv.value == value);
                if (value_view)
                    text = value_view.view;
            }

            if (dataset['usered_data']) {
                const { x } = (<Chart.ScatterDataPoint>item.dataset.data[item.dataIndex]);
                const user_id = dataset['usered_data'][x];
                if (dataset['usered_data'][x]) {
                    for (const user of members) {
                        if (user.id === user_id) {
                            text += ' User: ' + user.name;
                            break;
                        }
                    }
                }
            }
            return dataset.label + ': ' + text;
        };
    }

    setViewportBounds(start: number, end: number, forceUpdate = true) {
        const ticks = this.chartOptions.options.scales.x;
        ticks.min = start;
        ticks.max = end;

        forceUpdate && this.update();
    }

    private applyDatasetChanges(changes?: KeyValueChanges<string, any>) {
        this.update();
    }

    private setupYAxisScale(axisKey: string, idx: string) {
        const axis = this.chartOptions.options.scales[idx];
        const key = this.getAxisLocalStorageKey(axisKey, idx);
        if (!key)
            return;

        const params = ChartItemComponent.loadScaleParamsFromLocalStorage(key);
        if (!params)
            return;

        axis.min = params.min;
        axis.max = params.max;
    }

    private storeScaleParamsToLocalStorage() {
        const { A: leftYAxis, B: rightYAxis } = this.chart.scales;
        const leftKey = this.getAxisLocalStorageKey(this.leftYAxisLsKey, 'A');
        const rightKey = this.getAxisLocalStorageKey(this.rightYAxisLsKey, 'B');

        leftKey && ChartItemComponent.saveScaleParamsToLocalStorage(leftKey, leftYAxis);
        rightKey && ChartItemComponent.saveScaleParamsToLocalStorage(rightKey, rightYAxis);
    }

    private getAxisLocalStorageKey(axisKey: string, axisId: string) {
        const dataset = this.chartInfo.data.datasets.find(ds => ds.yAxisID === axisId);
        if (!dataset)
            return null;

        const { group_id } = dataset.dev_item || dataset.param;
        return `${axisKey}_${group_id}`;
    }

    private static loadScaleParamsFromLocalStorage(axisKey: string): { min: number, max: number } {
        const json = localStorage.getItem(axisKey);
        if (!json)
            return null;
        return JSON.parse(json);
    }

    private static saveScaleParamsToLocalStorage(axisKey: string, axis: any) {
        localStorage.setItem(axisKey, JSON.stringify({
            min: axis.min,
            max: axis.max,
        }));
    }

    private getAxes(): Axis_Config[] {
        return Object.keys(this.chart.scales)
            .map((key) => {
                const scaleItem = this.chart.scales[key] as LinearScale;
                return {
                    id: key,
                    isRight: scaleItem.position === 'right',
                    from: scaleItem.min,
                    to: scaleItem.max,
                    order: 0,
                    stepped: null,
                    display: scaleItem.options.display as 'auto' | false,
                    displayGrid: scaleItem.options.grid.display,
                };
            });
    }
}
