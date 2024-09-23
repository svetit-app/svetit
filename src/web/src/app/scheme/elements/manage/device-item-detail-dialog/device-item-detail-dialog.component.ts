import {Component, Inject} from '@angular/core';
import { UntypedFormArray, UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {Device, Device_Item, Device_Item_Group, Device_Item_Type, Plugin_Type} from '../../../scheme';
import { MAT_DIALOG_DATA, MatDialog, MatDialogRef, MatDialogTitle, MatDialogContent } from '@angular/material/dialog';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import {DeviceDetailDialogComponent} from '../device-detail-dialog/device-detail-dialog.component';
import {DeviceItemTypeDetailDialogComponent} from '../device-item-type-detail-dialog/device-item-type-detail-dialog.component';
import {DeviceItemGroupDetailDialogComponent} from '../device-item-group-detail-dialog/device-item-group-detail-dialog.component';
import {WithPlugin} from '../../../with-plugin.class';
import {SettingsService} from '../../../settings.service';
import {Observable} from 'rxjs/Observable';
import {PaginatorApi} from '../../../../user';
import {applyMixins} from 'rxjs/internal-compatibility';

import { CdkScrollable } from '@angular/cdk/scrolling';
import { MatFormField, MatLabel, MatHint } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatSelect } from '@angular/material/select';
import { MatOption } from '@angular/material/core';
import { MatButton } from '@angular/material/button';

export type Device_Item_Details = Pick<Device_Item, "name" | "device_id" | "type_id" | "extra" | "parent_id">;

@Component({
    selector: 'app-device-item-detail-dialog',
    templateUrl: './device-item-detail-dialog.component.html',
    styleUrls: ['./device-item-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [MatDialogTitle, ReactiveFormsModule, CdkScrollable, MatDialogContent, MatFormField, MatLabel, MatInput, MatSelect, MatOption, MatHint, MatButton]
})
export class DeviceItemDetailDialogComponent extends DetailDialog<Device_Item, DeviceItemDetailDialogComponent> implements WithPlugin<Device_Item> {
    readonly keys = Object.keys;

    disableChangeGroupId: boolean;
    disableDeviceIdChanging: boolean;

    pluginId: number;
    plugins: Plugin_Type[];
    editingExtraFields: { title: string, value: string }[];
    extraFields: UntypedFormArray;

    devItemTypes: Device_Item_Type[];
    devices: Device[];
    groups: Device_Item_Group[];

    constructor(
        fb: UntypedFormBuilder,
        @Inject(MAT_DIALOG_DATA) devItem: Device_Item & { disableChangeGroupId: boolean; disableDeviceIdChanging: boolean; },
        dialogRef: MatDialogRef<DeviceItemDetailDialogComponent>,
        schemeService: SchemeService,
        private dialog: MatDialog,
        settingsService: SettingsService,
    ) {
        super(dialogRef, devItem, schemeService, Structure_Type.ST_DEVICE_ITEM, fb, false);
        this.disableChangeGroupId = devItem.disableChangeGroupId;
        this.disableDeviceIdChanging = devItem.disableDeviceIdChanging;

        this.devItemTypes = this.schemeService.scheme.device_item_type;
        this.devices = this.schemeService.scheme.device;
        this.groups = this.schemeService.scheme.section
            .map(sct => sct.groups)
            .reduce((prev, curr) => {
                prev.push(...curr);
                return prev;
            }, []);

        this.init(settingsService)
            .subscribe(() => {
                super.initFg(devItem);
            });
    }

    init: (settingsService: SettingsService) => Observable<PaginatorApi<Plugin_Type>>;
    pluginChanged: (pluginId: number, extra: string, isItem: boolean) => void;

    createFormGroup(): UntypedFormGroup {
        this.buildExtraFields();

        const fg = this.fb.group({
            id: [null, []],
            name: ['', []],
            device_id: ['', Validators.required],
            type_id: [null, [Validators.required]],
            group_id: [null, [Validators.required]],
            extra: this.extraFields,
            parent_id: [null, []],
        });

        fg.controls['device_id'].valueChanges.subscribe((deviceId: number) => this.setDeviceId(deviceId));

        this.extraFields.valueChanges.subscribe((v: any[]) => {
            v.forEach((val, idx) => this.editingExtraFields[idx].value = val);
        });

        return fg;
    }

    patchValue(dialogData: Device_Item) {
        this.setDeviceId(dialogData?.device_id);

        let extra = [];
        if (dialogData?.extra) {
            extra = JSON.parse(dialogData.extra);
        }

        super.patchValue({
            ...dialogData,
            extra,
        });
    }

    createItem(formValue: any): Device_Item {
        const devItem = new Device_Item();

        Object.assign(devItem, formValue, {
            extra: JSON.stringify(devItem.extra || []),
        });
        devItem.type = this.devItemTypes.find(t => t.id === devItem.type_id);

        return devItem;
    }

    newDevice() {
        this.dialog.open(DeviceDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((device?: Device) => {});
    }

    newGroup() {
        this.dialog.open(DeviceItemGroupDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((dig?: Device_Item_Group) => {});
    }

    newItemType() {
        let data = {};
        if (this.disableChangeGroupId) {
            const group_type_id = this.groups.find(group => group.id === this.fg.controls['group_id'].value)?.type_id;
            if (group_type_id) {
                data = {group_type_id, disableGroupTypeChanging: true};
            }
        }

        this.dialog.open(DeviceItemTypeDetailDialogComponent, { width: '80%', data })
            .afterClosed()
            .subscribe((deviceItemType?: Device_Item_Type) => {});
    }

    private buildExtraFields() {
        this.extraFields = this.fb.array(
            (this.editingExtraFields || []).map((v) => [v.value || '', []]),
        );
    }

    private setDeviceId(deviceId: number) {
        const device = this.devices.find(dev => dev.id === +deviceId);
        if (device) {
            this.pluginId = device.plugin_id;
        } else {
            this.pluginId = 0;
        }

        this.pluginChanged(this.pluginId, this.fg.controls['extra'].value, true);
        this.buildExtraFields();

        this.fg.setControl('extra', this.extraFields);
    }
}

applyMixins(DeviceItemDetailDialogComponent, [WithPlugin]);
