import { Component, OnInit } from '@angular/core';
import { HttpResponse } from '@angular/common/http';
import { UntypedFormBuilder, UntypedFormGroup, Validators } from '@angular/forms';

//import { MAT_MOMENT_DATE_FORMATS, MomentDateAdapter } from '@angular/material-moment-adapter';
import {DateAdapter, MAT_DATE_FORMATS, MAT_DATE_LOCALE, NativeDateAdapter} from '@angular/material/core';

import { Device_Item_Type, DIG_Type, Section, Device_Item } from "../../scheme";
import { SchemeService, ExportConfig, ExportItem } from "../../scheme.service";
import { SchemesService } from '../../../schemes/schemes.service';
import { Scheme } from "../../../user";
import {ActivatedRoute} from '@angular/router';
import {TranslateService} from '@ngx-translate/core';

import moment from 'moment-timezone';
import { Moment } from 'moment';

export class RussianDateAdapter extends NativeDateAdapter {
  parse(value: any): Date | null {
    if ((typeof value === 'string') && (value.indexOf('/') > -1)) {
      const str = value.split('.');
      if (str.length < 2 || isNaN(+str[0]) || isNaN(+str[1]) || isNaN(+str[2])) {
        return null;
      }
      return new Date(Number(str[2]), Number(str[1]) - 1, Number(str[0]), 12);
    }
    const timestamp = typeof value === 'number' ? value : Date.parse(value);
    return isNaN(timestamp) ? null : new Date(timestamp);
  }
}

interface TimeZone {
  title: string;
  offset: string;
  value: string;
}

@Component({
  selector: 'app-export',
  templateUrl: './export.component.html',
  styleUrls: ['./export.component.css'],
/*  providers: [
    // The locale would typically be provided on the root module of your application. We do it at
    // the component level here, due to limitations of our example generation script.
    {provide: MAT_DATE_LOCALE, useValue: 'ru-RU'},

    // `MomentDateAdapter` and `MAT_MOMENT_DATE_FORMATS` can be automatically provided by importing
    // `MatMomentDateModule` in your applications root module. We provide it at the component level
    // here, due to limitations of our example generation script.
    {provide: DateAdapter, useClass: MomentDateAdapter, deps: [MAT_DATE_LOCALE]},
    {provide: MAT_DATE_FORMATS, useValue: MAT_MOMENT_DATE_FORMATS},
  ],*/
})
export class ExportComponent implements OnInit {
  firstFormGroup: UntypedFormGroup;
  secondFormGroup: UntypedFormGroup;
  dataFormGroup: UntypedFormGroup;

  schemes: Scheme[] = [];
  types: Device_Item_Type[];

  loading: boolean = false;

  dataPreselected: number[] = [];

  locale: string;
  tzs: TimeZone[];

  constructor(
    private _formBuilder: UntypedFormBuilder,
    private schemeService: SchemeService,
    private schemesService: SchemesService,
    private route: ActivatedRoute,
    private dateAdapter: DateAdapter<Date>,
    public translate: TranslateService
  ) {
    this.locale = this.translate.currentLang;
    this.dateAdapter.setLocale(this.locale);
    this.dateAdapter.getFirstDayOfWeek = () => 1;

    const tzNames = moment.tz.names();
    this.tzs = tzNames.map((tzname) => {
      const offset = moment.tz(tzname).format('Z');
      return {
          title: `${tzname} (UTC${offset})`,
          offset: offset,
          value: tzname
        };
      });
  }

  ngOnInit() {
    this.loading = false;

    let date_from_d = new Date();
    date_from_d.setDate(1);
    date_from_d.setHours(0);
    date_from_d.setMinutes(0);
    date_from_d.setSeconds(0);
    date_from_d.setMilliseconds(0);
    let date_to_d = new Date();
    date_to_d.setHours(23);
    date_to_d.setMinutes(59);
    date_to_d.setSeconds(59);
    date_to_d.setMilliseconds(0);

    const usrTz = moment.tz.guess();

    this.firstFormGroup = this._formBuilder.group({
      schemes: [[this.schemeService.scheme.id], Validators.required],
    });
    this.secondFormGroup = this._formBuilder.group({
      date_from: [date_from_d, Validators.required],
      date_to: [date_to_d, Validators.required],
      timezone: [usrTz, Validators.required]
    });
    this.dataFormGroup = this._formBuilder.group({
      hide_null: [true],
      items: [[], Validators.required],
    });

    this.types = this.schemeService.scheme.device_item_type;

    this.dataPreselected = this.route.snapshot.data['dataPreselected'];
    this.dataFormGroup.get('items').setValue(this.dataPreselected);
  }

  fillSchemes(current_only: boolean): void {
    if (current_only)
      this.firstFormGroup.controls.schemes.setValue([this.schemeService.scheme.id]);
    else if (this.schemes.length === 0)
      this.schemesService.getSchemes(1000, 0, 'title')
        .subscribe(data => this.schemes = data.results);
  }

  onSubmit(): void {
    this.loading = true;

    const ts_obj = {
      ts_from: +this.secondFormGroup.value.date_from,
      ts_to: +this.secondFormGroup.value.date_to,
      timezone: this.secondFormGroup.value.timezone
    };

    const data: ExportConfig = Object.assign(this.firstFormGroup.value, ts_obj, this.dataFormGroup.value);
    this.schemeService.exportExcel(data).subscribe((response: HttpResponse<Blob>) => {
      this.loading = false;
      if (!response) {
        console.error("Fail to get excel");
        return;
      }

      let contentDispositionHeader = response.headers.get('Content-Disposition');
      let result = contentDispositionHeader.split(';')[1].trim().split('=')[1];

      let url = window.URL.createObjectURL(response.body);
      let anchor = document.createElement('a');
      anchor.style.display = "none";
      anchor.download = result.replace(/"/g, '');
      anchor.href = url;
      anchor.dataset.downloadurl = ['text/plain', anchor.download, anchor.href].join(':');
      document.body.appendChild(anchor);
      anchor.click();

      document.body.removeChild(anchor);
      window.URL.revokeObjectURL(url);
      //window.open(url);
    });
 }
}
