import {Component, Input, OnChanges, OnInit} from '@angular/core';

import {Device, Device_Item, Device_Item_Type, Plugin_Type, Section} from '../../scheme';
import {SchemeService} from '../../scheme.service';

import {ChangeInfo, ChangeState, ChangeTemplate, Structure_Type} from '../settings';
import {UIService} from '../../../ui.service';
import {SettingsService} from '../../settings.service';
import {applyMixins} from 'rxjs/internal-compatibility';
import {WithPlugin} from '../../with-plugin.class';
import {Observable} from 'rxjs/Observable';
import {PaginatorApi} from '../../../user';

@Component({
    selector: 'app-devices',
    templateUrl: './devices.component.html',
    styleUrls: ['../settings.css', './devices.component.css']
})
export class DevicesComponent extends ChangeTemplate<Device> implements WithPlugin<Device>, OnInit {
    constructor(
        schemeService: SchemeService,
        ui: UIService,
        settingsService: SettingsService,
    ) {
        super(schemeService, Device, Structure_Type.ST_DEVICE, ui);
        this.init(settingsService)
            .subscribe(() => {});
    }

    pluginChanged: (pluginId: number, extra: string, isItem: boolean) => void;
    init: (settingsService: SettingsService) => Observable<PaginatorApi<Plugin_Type>>;
    plugins: Plugin_Type[];
    editingExtraFields: { title: string, value: string }[];

    getObjects(): Device[] {
        return this.schemeService.scheme.device;
    }

    ngOnInit() {
        this.fillItems();
    }

    select(item: ChangeInfo<Device>) {
        super.select(item);
        if (!this.sel_item) {
            return;
        }

        this.pluginChanged(item.obj.plugin_id, item.obj.extra, false);
    }

    initItem(obj: Device): void {
        obj.extra = null;
        obj.check_interval = 0;
        this.pluginChanged(obj.plugin_id, obj.extra, false);
    }

    public extraChanged(idx: number, val: string) {
        this.editingExtraFields[idx].value = val;
        this.sel_item.obj.extra = JSON.stringify(this.editingExtraFields.map(f => f.value));
        this.itemChanged();
    }
}

applyMixins(DevicesComponent, [WithPlugin]);

@Component({
    selector: 'app-deviceitems',
    templateUrl: './deviceitems.component.html',
    styleUrls: ['../settings.css', './devices.component.css']
})
export class DeviceItemsComponent extends ChangeTemplate<Device_Item> implements OnInit, OnChanges, WithPlugin<Device_Item> {
    @Input() dev: Device;

    itemtypes: Device_Item_Type[];
    sections: Section[];
    private groupSelected = false;
    private typeSelected = false;

    init: (settingsService: SettingsService) => Observable<PaginatorApi<Plugin_Type>>;
    pluginChanged: (pluginId: number, extra: string, isItem: boolean) => void;
    plugins: Plugin_Type[];
    editingExtraFields: { title: string, value: string }[];

    constructor(
        schemeService: SchemeService,
        ui: UIService,
        settingsService: SettingsService,
    ) {
        super(schemeService, Device_Item, Structure_Type.ST_DEVICE_ITEM, ui);
        this.init(settingsService)
            .subscribe(() => {});
    }

    getObjects(): Device_Item[] {
        return this.dev.items;
    }

    ngOnInit() {
        this.initComponent();
    }

    ngOnChanges() {
        this.initComponent()
    }

    title(item: Device_Item): string {
        if (!item) {
            item = this.sel_item.obj;
        }
        if (item.name.length) {
            return item.name;
        } else if (item.type && item.type.title.length) {
            return item.type.title;
        }
        return '';
    }

    typeChanged(): void {
        this.itemChanged();

        if (this.sel_item.obj.type_id > 0) {
            for (const itemtype of this.itemtypes) {
                if (this.sel_item.obj.type_id === itemtype.id) {
                    this.sel_item.obj.type = itemtype;
                    break;
                }
            }
        }

        this.filterGroups();
    }

    groupChanged(): void {
        this.itemChanged();

        if (this.typeSelected) {
            return;
        }
        this.groupSelected = true;
        let group;
        if (!group) {
            this.itemtypes = this.schemeService.scheme.device_item_type;
            return;
        }
        this.schemeService.scheme.section
            .find((section) => {
                group = section.groups.find(gr => gr.id === this.sel_item.obj.group_id);
                return !!group;
            });

        this.itemtypes = this.schemeService.scheme.device_item_type.filter(type => type.group_type_id === group.type_id);
        const foundType = !!this.itemtypes.find(itemType => itemType.id === this.sel_item.obj.type_id);

        if (!foundType) {
            this.sel_item.obj.type_id = 0;
        }
    }

    initItem(obj: Device_Item): void {
        obj.name = '';
        obj.device_id = this.dev.id;
        obj.type_id = 0;
        obj.extra = null;
        this.pluginChanged(this.dev.plugin_id, obj.extra, true);
    }

    select(item: ChangeInfo<Device_Item>) {
        this.reset();
        super.select(item);
        if (!this.sel_item) {
            return;
        }

        this.pluginChanged(this.dev.plugin_id, this.sel_item.obj.extra, true);
    }

    cancel(evnt: any = undefined) {
        this.sel_item.obj.type_id = this.sel_item.prev.type_id; // restore type_id and group_id if it reset
        this.sel_item.obj.group_id = this.sel_item.prev.group_id;

        super.cancel(evnt);
    }

    addItem(obj: Device_Item, select: boolean = true) {
        let item = {state: ChangeState.NoChange, obj: obj, prev: {...obj}} as ChangeInfo<Device_Item>;
        this.items.push(item);
        if (select) {
            this.sel_item = item;
        }
    }

    private filterGroups() {
        if (this.groupSelected) {
            return;
        }

        this.typeSelected = true;

        if (!this.sel_item.obj.type) {
            this.sections = this.schemeService.scheme.section;
        }

        let found = false;
        this.sections = this.schemeService.scheme.section
            .map((section) => {
                const groups = section.groups.filter(group => group.type_id === this.sel_item.obj.type.group_type_id);
                if (!groups.length) {
                    return null;
                }

                if (!!groups.find(g => g.id === this.sel_item.obj.group_id)) {
                    found = true;
                }

                return {
                    ...section,
                    groups,
                };
            })
            .filter(t => !!t);

        if (!found) {
            this.sel_item.obj.group_id = 0;
        }
    }

    public extraChanged(idx: number, val: string) {
        this.editingExtraFields[idx].value = val;
        this.sel_item.obj.extra = JSON.stringify(this.editingExtraFields.map(f => f.value));
        this.itemChanged();
    }

    private initComponent() {
        this.reset();
        this.fillItems();
    }

    private reset() {
        this.groupSelected = false;
        this.typeSelected = false;
        this.itemtypes = this.schemeService.scheme.device_item_type;
        this.sections = this.schemeService.scheme.section;
    }
}

applyMixins(DeviceItemsComponent, [WithPlugin]);
