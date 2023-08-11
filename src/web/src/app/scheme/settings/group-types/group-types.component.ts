import {Component, Directive, Input, OnChanges, OnInit} from '@angular/core';

import {SchemeService} from '../../scheme.service';
import {Device_Item_Type, DIG_Param_Type, DIG_Status_Type, DIG_Type, Save_Timer, Sign_Type} from '../../scheme';

import {ChangeTemplate, Structure_Type} from '../settings';

import {SettingsService} from '../../settings.service';
import {UIService} from '../../../ui.service';
import {addParamsToGroups} from '../../add-params-helpers';

@Component({
  selector: 'app-group-types',
  templateUrl: './group-types.component.html',
  styleUrls: ['../settings.css', './group-types.component.css']
})
export class GroupTypesComponent extends ChangeTemplate<DIG_Type> implements OnInit
{
  constructor(
    schemeService: SchemeService,
    private settingsService: SettingsService,
    ui: UIService,
  ) {
    super(schemeService, DIG_Type, Structure_Type.ST_DIG_TYPE, ui);
  }

  getObjects(): DIG_Type[] {
    return this.schemeService.scheme.dig_type;
  }

  ngOnInit() {
    this.fillItems();
  }
}

@Component({
  selector: 'app-item-types',
  templateUrl: './item-types.component.html',
  styleUrls: ['../settings.css', './group-types.component.css']
})
export class ItemTypesComponent extends ChangeTemplate<Device_Item_Type> implements OnInit, OnChanges {
  @Input() grouptype: DIG_Type;

  signTypes: Sign_Type[];
  save_timers: Save_Timer[];

  constructor(
    schemeService: SchemeService,
    private settingsService: SettingsService,
    ui: UIService,
  ) {
    super(schemeService, Device_Item_Type, Structure_Type.ST_DEVICE_ITEM_TYPE, ui);
  }

  ngOnChanges() {
    this.ngOnInit();
  }

  getObjects(): Device_Item_Type[] {
    return this.schemeService.scheme.device_item_type.filter(obj => obj.group_type_id === this.grouptype.id);
  }

  ngOnInit() {
    this.settingsService.getSaveTimers().subscribe(api => {
      this.save_timers = api.results;
      this.signTypes = this.schemeService.scheme.sign_type;
      this.fillItems();
    });
  }

  initItem(obj: Device_Item_Type): void {
    obj.name = '';
    obj.title = '';
    obj.group_type_id = this.grouptype.id;
  }
}

@Component({
  selector: 'app-param-types',
  templateUrl: './param-types.component.html',
  styleUrls: ['../settings.css', './group-types.component.css']
})
export class ParamTypesComponent extends ChangeTemplate<DIG_Param_Type> implements OnInit {
  @Input() grouptype: DIG_Type;

  constructor(
    schemeService: SchemeService,
    ui: UIService,
  ) {
    super(schemeService, DIG_Param_Type, Structure_Type.ST_DIG_PARAM_TYPE, ui);
  }

  getObjects(): DIG_Param_Type[] {
    return this.schemeService.scheme.dig_param_type.filter(obj => obj.group_type_id === this.grouptype.id);
  }

  ngOnInit() {
    this.fillItems();
  }

  save(event: any = undefined) {
      super.saveImpl(event)
          .subscribe((data) => {
              if (data.inserted?.length < 1) return;

              let paramsNames = data.inserted.map(t => (t as any).title).join(',');
              this.ui.confirmationDialog(`Добавить уставку(и) ${paramsNames} в контуры управления с типом "${this.grouptype.title}"?`)
                  .subscribe((confirmation) => { // TODO: i18n
                      if (!confirmation) return;

                      const paramTypesIds = data.inserted.map(t => t.id);
                      addParamsToGroups(this.schemeService, this.grouptype.id, paramTypesIds)
                          .subscribe(() => {});
                  });
          });
  }

  initItem(obj: DIG_Param_Type): void {
    obj.name = '';
    obj.title = '';
    obj.group_type_id = this.grouptype.id;
  }
}

@Component({
  selector: 'app-statuses',
  templateUrl: './statuses.component.html',
  styleUrls: ['../settings.css', './group-types.component.css']
})
export class StatusesComponent extends ChangeTemplate<DIG_Status_Type> implements OnInit {
  @Input() grouptype: DIG_Type;

  constructor(
    schemeService: SchemeService,
    ui: UIService,
  ) {
    super(schemeService, DIG_Status_Type, Structure_Type.ST_DIG_STATUS_TYPE, ui);
  }

  getObjects(): DIG_Status_Type[] {
    return this.schemeService.scheme.dig_status_type.filter(obj => obj.group_type_id === this.grouptype.id);
  }

  ngOnInit() {
    this.fillItems();
  }

  initItem(obj: DIG_Status_Type): void {
    obj.name = '';
    obj.group_type_id = this.grouptype.id;
  }
}

// Deprecated
@Directive()
export class OtherTypesComponent implements OnInit {
  paramItems: DIG_Param_Type[];
  itemTypes: Device_Item_Type[];
  groupTypes: DIG_Type[];
  statuses: DIG_Status_Type[];

  status: DIG_Status_Type;
  param: DIG_Param_Type;
  sel_itype: Device_Item_Type;
  sel_gtype: DIG_Type;

  constructor(
    private schemeService: SchemeService,
  ) {}

  ngOnInit() {
    this.paramItems = this.schemeService.scheme.dig_param_type;
    this.groupTypes = this.schemeService.scheme.dig_type;
    this.statuses = [];
  }

  select(gtype: DIG_Type): void {
    if (this.sel_gtype == gtype) {
      this.sel_gtype = undefined;
      this.sel_itype = undefined;
    } else {
      this.sel_gtype = gtype;

      this.itemTypes = [];
      let itemTypes = this.schemeService.scheme.device_item_type;
      for (let itype of itemTypes)
        if (itype.group_type_id == this.sel_gtype.id)
          this.itemTypes.push(itype);
    }
  }

  remove(gtype: DIG_Type): void {
    // Dialog
  }

  add(): void {
    this.sel_gtype = new DIG_Type();
    this.sel_gtype.id = 0;
    this.groupTypes.push(this.sel_gtype);
  }

  save(): void {
    this.sel_gtype.name = this.sel_gtype.name.trim();
    if (!this.sel_gtype.name) return;
  }

  select_it(itype: Device_Item_Type): void {
    this.sel_itype = this.sel_itype == itype ? undefined : itype;
  }

  remove_it(itype: Device_Item_Type): void {
    // Dialog
  }

  add_it(): void {
    this.sel_itype = new Device_Item_Type();
    this.sel_itype.id = 0;
    this.sel_itype.title = '';
    this.sel_itype.group_type_id = this.sel_gtype.id;
    this.itemTypes.push(this.sel_itype);
  }

  save_it(): void {
    this.sel_itype.name = this.sel_itype.name.trim();
    if (!this.sel_itype.name) return;
  }

  select_param(param: DIG_Param_Type): void {
    this.param = this.param == param ? undefined : param;
  }

  remove_param(param: DIG_Param_Type): void {
    // Dialog
  }

  add_param(): void {
    this.param = new DIG_Param_Type();
    this.param.id = 0;
    this.paramItems.push(this.param);
  }

  save_param(): void {
    this.param.name = this.param.name.trim();
    if (!this.param.name) return;
  }

  select_status(status: DIG_Status_Type): void {
    this.status = this.status == status ? undefined : status;
  }

  remove_status(status: DIG_Status_Type): void {
    // Dialog
  }

  add_status(): void {
    this.status = new DIG_Status_Type();
    this.status.id = 0;
    this.statuses.push(this.status);
  }

  save_status(): void {
    this.status.name = this.status.name.trim();
    if (!this.status.name) return;
  }
}
