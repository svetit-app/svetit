import { Component, Input, OnChanges, OnInit, inject } from '@angular/core';

import {Device, Device_Item, Device_Item_Type, Plugin_Type, Section} from '../../scheme';
import {SchemeService} from '../../scheme.service';

import {ChangeInfo, ChangeState, ChangeTemplate, Structure_Type} from '../settings';
import {UIService} from '../../../ui.service';
import {SettingsService} from '../../settings.service';
import {applyMixins} from 'rxjs/internal-compatibility';
import {WithPlugin} from '../../with-plugin.class';
import {Observable} from 'rxjs/Observable';
import {PaginatorApi} from '../../../user';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatIconModule } from '@angular/material/icon';
import { FormsModule } from '@angular/forms';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSelectModule } from '@angular/material/select';
import { DeviceItemsComponent } from './deviceitems/component';

@Component({
	selector: 'app-devices',
	templateUrl: './devices.component.html',
	styleUrls: ['../settings.css', './devices.component.css'],
	standalone: true,
	imports: [
		FormsModule, MatExpansionModule, MatIconModule, MatFormFieldModule, MatSelectModule,
		DeviceItemsComponent,
	]
})
export class DevicesComponent extends ChangeTemplate<Device> implements WithPlugin<Device>, OnInit {
    constructor() {
        const schemeService = inject(SchemeService);
        const ui = inject(UIService);
        const settingsService = inject(SettingsService);

        super(schemeService, Device, Structure_Type.ST_DEVICE, ui);
        this.init(settingsService)
            .subscribe(() => {});
    }

    pluginChanged: (pluginId: number, extra: string | Array<string>, isItem: boolean) => void;
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
