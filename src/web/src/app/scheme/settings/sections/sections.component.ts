import { Component, Input, OnChanges, OnInit, SimpleChanges, inject } from '@angular/core';

import {SchemeService} from '../../scheme.service';
import {Device_Item_Group, DIG_Param, DIG_Param_Type, DIG_Type, Section} from '../../scheme';
import {SettingsService} from '../../settings.service';
import {ChangeInfo, ChangeState, ChangeTemplate, Structure_Type} from '../settings';
import {UIService} from '../../../ui.service';
import {filter, switchMap} from 'rxjs/operators';
import {addAllGroupParamsToDigImpl} from '../../add-params-helpers';

@Component({
    selector: 'app-sections',
    templateUrl: './sections.component.html',
    styleUrls: ['../settings.css', './sections.component.css']
})
export class SectionsComponent extends ChangeTemplate<Section> implements OnInit {
    private settingsService = inject(SettingsService);

    constructor() {
        const schemeService = inject(SchemeService);
        const ui = inject(UIService);

        super(schemeService, Section, Structure_Type.ST_SECTION, ui);
    }

    getObjects(): Section[] {
        return this.schemeService.scheme.section;
    }

    ngOnInit() {
        this.fillItems();

        /*
        // Dialog
        this.sections = this.sections.filter(s => s !== sct);
        this.settingsService.deleteSection(sct).subscribe(_ => {
          // this.controlService.deleteSection(sct);
        });*/
    }
}

@Component({
    selector: 'app-groups',
    templateUrl: './groups.component.html',
    styleUrls: ['../settings.css', './sections.component.css']
})
export class GroupsComponent extends ChangeTemplate<Device_Item_Group> implements OnInit {
    @Input() sct: Section;

    groupTypes: DIG_Type[];
    sections: Section[];

    constructor() {
        const schemeService = inject(SchemeService);
        const ui = inject(UIService);

        super(schemeService, Device_Item_Group, Structure_Type.ST_DEVICE_ITEM_GROUP, ui);
    }

    getObjects(): Device_Item_Group[] {
        return this.sct.groups;
    }

    ngOnInit() {
        this.groupTypes = this.schemeService.scheme.dig_type;
        this.sections = this.schemeService.scheme.section;
        this.fillItems();
    }

    title(item: Device_Item_Group = undefined): string {
        if (item === undefined) {
            item = this.sel_item.obj;
        }
        return item.title ? item.title : (item.type ? item.type.title : '');
    }

    initItem(obj: Device_Item_Group): void {
        obj.section_id = this.sct.id;
        obj.params ??= [];
        obj.items ??= [];
    }

    save(event: any = undefined) {
        super.saveImpl(event)
            .subscribe((data) => {
                if (data.inserted?.length < 1) return;

                this.ui.confirmationDialog('Добавить все уставки из типа контура?') // TODO: i18n
                    .subscribe((confirmation) => {
                        if (!confirmation) return;

                        data.inserted.forEach((dig: Device_Item_Group) => {
                            addAllGroupParamsToDigImpl(this.schemeService, dig.id, dig.type_id);
                        });
                    });
            });
    }

    typeChanged(obj: Device_Item_Group) {
        obj.type = this.groupTypes.find(t => t.id === obj.type_id);
    }
}

@Component({
    selector: 'app-params-in-group',
    templateUrl: './params-in-group.component.html',
    styleUrls: ['../settings.css', './sections.component.css']
})
export class ParamsInGroupComponent extends ChangeTemplate<DIG_Param> implements OnInit, OnChanges {
    @Input() group: Device_Item_Group;

    params: DIG_Param_Type[];

    constructor() {
        const schemeService = inject(SchemeService);
        const ui = inject(UIService);

        super(schemeService, DIG_Param, Structure_Type.ST_DIG_PARAM, ui);
    }

    ngOnInit() {
        this.init();
    }

    ngOnChanges(changes: SimpleChanges) {
        if (changes.group) {
            this.init();
        }
    }

    paramTypeChanged(obj: DIG_Param) {
        obj.param_id = obj.param.id;
        this.itemChanged();
    }

    initItem(obj: DIG_Param): void {
        obj.param = new DIG_Param_Type();
        obj.group_id = this.group.id;
    }

    init() {
        this.fillItems();
        this.params = this.schemeService.scheme.dig_param_type.filter(obj => obj.group_type_id === this.group.type_id);
    }

    getObjects(): DIG_Param[] {
        const arr: DIG_Param[] = [];
        const add = (items: DIG_Param[]) => {
            for (const item of items) {
                arr.push(item);
                if (item.childs) {
                    add(item.childs);
                }
            }
        };

        add(this.group.params);
        return arr;
    }
}
