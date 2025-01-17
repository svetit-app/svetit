import { Component, inject } from '@angular/core';
import { UntypedFormArray, UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {Device, Plugin_Type} from '../../../scheme';
import {MAT_DIALOG_DATA, MatDialog, MatDialogRef} from '@angular/material/dialog';
import {SettingsService} from '../../../settings.service';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import {PluginDetailDialogComponent} from '../plugin-detail-dialog/plugin-detail-dialog.component';
import {WithPlugin} from '../../../with-plugin.class';
import {applyMixins} from 'rxjs/internal-compatibility';
import {Observable} from 'rxjs/Observable';
import {PaginatorApi} from '../../../../user';

import { MatFormField, MatLabel, MatHint } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatSelect } from '@angular/material/select';
import { MatOption } from '@angular/material/core';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-device-detail-dialog',
    templateUrl: './device-detail-dialog.component.html',
    styleUrls: ['./device-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [ReactiveFormsModule, MatFormField, MatLabel, MatInput, MatSelect, MatOption, MatHint, MatButton]
})
export class DeviceDetailDialogComponent extends DetailDialog<Device, DeviceDetailDialogComponent> implements WithPlugin<Device> {
    private dialog = inject(MatDialog);
    private settingsService = inject(SettingsService);

    readonly keys = Object.keys;
    extraFields: UntypedFormArray;
    plugins: Plugin_Type[];
    editingExtraFields: { title: string; value: string; }[];

    constructor() {
        const fb = inject(UntypedFormBuilder);
        const dev = inject<Device>(MAT_DIALOG_DATA);
        const dialogRef = inject<MatDialogRef<DeviceDetailDialogComponent>>(MatDialogRef);
        const schemeService = inject(SchemeService);

        super(dialogRef, dev, schemeService, Structure_Type.ST_DEVICE, fb, false);
        const settingsService = this.settingsService;

        this.init(settingsService)
            .subscribe(() => super.initFg(dev));
    }

    public init: (settingsService: SettingsService) => Observable<PaginatorApi<Plugin_Type>>;
    public pluginChanged: (pluginId: number, extra: string | Array<any>, isItem?: boolean) => void;

    createFormGroup(): UntypedFormGroup {
        this.buildExtraFields();

        const fg = this.fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
            plugin_id: [0, []],
            check_interval: [50, [Validators.min(50)]],
            extra: this.extraFields,
        });

        fg.controls['plugin_id'].valueChanges.subscribe((pluginId: number) => this.setPlugin(pluginId));

        this.extraFields.valueChanges.subscribe((v: any[]) => {
            v.forEach((val, idx) => this.editingExtraFields[idx].value = val);
        });

        return fg;
    }

    patchValue(dialogData: Device) {
        this.setPlugin(dialogData?.plugin_id);

        let extra = [];
        if (dialogData?.extra) {
            extra = JSON.parse(dialogData.extra);
        }

        super.patchValue({
            ...dialogData,
            plugin_id: dialogData?.plugin_id || 0, // if plugin_id === null, set it to 0
            extra,
        });
    }

    createItem(formValue: any): Device {
        return {
            ...formValue,
            extra: JSON.stringify(formValue.extra || null),
            items: [],
        };
    }

    newPlugin() {
        this.dialog.open(PluginDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((pluginType: Plugin_Type) => {
                this.plugins.push(pluginType);
            });
    }

    private buildExtraFields() {
        this.extraFields = this.fb.array(
            (this.editingExtraFields || []).map((v) => [v.value || '', []]),
        );
    }

    private setPlugin(pluginId: number) {
        this.pluginChanged(pluginId, this.fg.controls['extra'].value, false);
        this.buildExtraFields();

        this.fg.setControl('extra', this.extraFields);
    }
}

applyMixins(DeviceDetailDialogComponent, [WithPlugin]);
