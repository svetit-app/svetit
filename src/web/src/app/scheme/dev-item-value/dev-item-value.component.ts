import { Component, Input, OnInit, OnDestroy, Inject } from '@angular/core';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import {TranslateService} from '@ngx-translate/core';

import {SubscriptionLike} from 'rxjs';

import { ControlService } from "../control.service";
import { AuthService } from "../../auth/service";
import {Device_Item, Register_Type, Value_View} from '../scheme';
import { SchemeService } from "../scheme.service";

import { VideoStreamDialogComponent } from "./video-stream-dialog/video-stream-dialog.component";

@Component({
  selector: 'app-dev-item-value',
  templateUrl: './dev-item-value.component.html',
  styleUrls: ['./dev-item-value.component.css']
})
export class DevItemValueComponent implements OnInit, OnDestroy {

  @Input() item: Device_Item;

  cantChange: boolean;
  is_toggle: boolean;
  is_holding: boolean;
  is_button: boolean;
  is_file: boolean;
  is_video: boolean;
  is_loading: boolean = false;

  timer_: number;
  changed_sub: SubscriptionLike = null;

  constructor(
	  public translate: TranslateService,
      public dialog: MatDialog,
      private controlService: ControlService,
      private user: AuthService,
	  private schemeService: SchemeService,
  ) { }

  ngOnInit() {
    this.cantChange = !this.user.canChangeValue();
    this.is_toggle = this.item.type.register_type == Register_Type.RT_COILS;
    this.is_holding = this.item.type.register_type == Register_Type.RT_HOLDING_REGISTERS;
	this.is_button = this.item.type.register_type == Register_Type.RT_SIMPLE_BUTTON;
	this.is_file = this.item.type.register_type == Register_Type.RT_FILE;
	this.is_video = this.item.type.register_type == Register_Type.RT_VIDEO_STREAM;
  }

  ngOnDestroy() {
    if (this.changed_sub)
      this.changed_sub.unsubscribe();
  }

  get sign_available(): boolean {
    return !this.is_toggle && this.item.val && this.item.val.value !== null && this.item.type.sign !== undefined && this.item.type.sign.name.length > 0;
  }

  get text_value(): string {
    const vv = this.item.type.views?.find(vv => vv.value === this.item.val?.value);
    if (vv)
        return vv.view;

    const val = this.item.val ? this.item.val.value : null;
    if (val === undefined || val === null)
        return this.translate.instant("NOT_CONNECTED");

    if (typeof(val) === 'object')
        return val[this.item.val.raw_value];

    if (this.item.type.register_type === Register_Type.RT_DISCRETE_INPUTS
        && typeof val === "boolean")
        return val ? '1' : '0';

    return val;
  }

  changed_finished(): void
  {
    this.is_loading = false;
    this.changed_sub.unsubscribe();
    this.changed_sub = null;
  }

  write(value: number | boolean): void {
    if (value !== undefined)
    {
      let old_value = Object.assign({}, this.item.val);
      this.item.val.value = value;

      this.is_loading = true;

      this.timer_ = <any>setTimeout(() => {
        this.item.val.raw_value = old_value.raw_value;
        this.item.val.value = old_value.value;
        this.changed_finished();
      }, 2500);

      this.changed_sub = this.controlService.dev_item_changed.subscribe(dev_item_list => {
        for (const item of dev_item_list)
        {
          if (item.id === this.item.id)
          {
            clearTimeout(this.timer_);
            this.changed_finished();
            break;
          }
        }
      });

      this.controlService.writeToDevItem(this.item.id, value);
    }
  }

  openDialog(): void {
    if (this.cantChange || !this.is_holding || this.is_loading)
      return;

    let dialogRef = this.dialog.open(HoldingRegisterDialogComponent, {
      width: '80%',
      data: this.item
    });

    dialogRef.afterClosed().subscribe(result => this.write(result));
  }

  click_button(): void
  {
      this.controlService.writeToDevItem(this.item.id, 0);
  }

    open_video(): void
    {
        let settings = VideoStreamDialogComponent.get_default_settings();
        settings['data'] = { isImg: false, devItem: this.item, img: null };
        let dialogRef = this.dialog.open(VideoStreamDialogComponent, settings);

        dialogRef.afterClosed().subscribe(result => console.log(result));
    }

  handleFileInput(files: FileList): void
  {
    this.is_loading = true;
	  this.schemeService.upload_file(this.item.id, files.item(0)).subscribe(
      data => {
        console.log("success");
        this.is_loading = false
      },
      error => {
        console.log(error);
        this.is_loading = false
      });
  }
}

@Component({
  selector: 'app-holding-register-dialog',
  templateUrl: './holding-register-dialog.component.html',
  styleUrls: ['./dev-item-value.component.css']
})
export class HoldingRegisterDialogComponent {
  value: any;
  values: any[];
  private _max: number = 100;
  get max(): number {
    if (this.value > this._max)
      this._max = this.value;
    return this._max;
  }

  check_is_string(): boolean
  {
    return typeof this.value == "string" && parseFloat(this.value).toString() != this.value;
  }

  get input_type(): string
  {
    if (!this.auto_detect || this.check_is_string())
    {
      return "text";
    }
    return "number";
  }

  auto_detect: boolean = true;

  constructor(
    public dialogRef: MatDialogRef<HoldingRegisterDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: Device_Item)
  {
    if (!data.val)
      return;
    if (typeof(data.val.value) === 'object') {
      this.value = data.val.raw_value;
      this.values = data.val.value;
    }
    else
      this.value = data.val.value;

    if (this.check_is_string())
    {
      this.auto_detect = false;
    }
  }

  onNoClick(): void {
    this.dialogRef.close();
  }

  set_string_type(): void
  {
    this.auto_detect = false;
  }
}
