import {Component, Inject} from '@angular/core';
import {MAT_DIALOG_DATA, MatDialog, MatDialogRef} from '@angular/material/dialog';
import {Device_Item_Group, DIG_Type} from '../../../scheme';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemeService} from '../../../scheme.service';
import {DeviceItemGroupTypeDetailDialogComponent} from '../device-item-group-type-detail-dialog/device-item-group-type-detail-dialog.component';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import { MatFormField, MatLabel, MatHint } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatSelect } from '@angular/material/select';
import { MatOption } from '@angular/material/core';

import { MatButton } from '@angular/material/button';

export type Device_Item_Group_Details = Pick<Device_Item_Group, "id" | "title" | "type_id" | "mode">;

@Component({
    selector: 'app-device-item-group-detail-dialog',
    templateUrl: './device-item-group-detail-dialog.component.html',
    styleUrls: ['./device-item-group-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [ReactiveFormsModule, MatFormField, MatLabel, MatInput, MatSelect, MatOption, MatHint, MatButton]
})
export class DeviceItemGroupDetailDialogComponent extends DetailDialog<Device_Item_Group, DeviceItemGroupDetailDialogComponent> {
    groupTypes: DIG_Type[];

    constructor(
        fb: UntypedFormBuilder,
        dialogRef: MatDialogRef<DeviceItemGroupDetailDialogComponent>,
        schemeService: SchemeService,
        private dialog: MatDialog,
        @Inject(MAT_DIALOG_DATA) devItemGroup: Device_Item_Group,
    ) {
        super(dialogRef, devItemGroup, schemeService, Structure_Type.ST_DEVICE_ITEM_GROUP, fb);
        this.groupTypes = this.schemeService.scheme.dig_type;
    }

    createFormGroup(): UntypedFormGroup {
        return this.fb.group({
            title: ['', []],
            type_id: [null, [Validators.required]],
            section_id: [null, []],
        });
    }

    createItem(formValue: any): Device_Item_Group {
        const group = new Device_Item_Group();
        Object.assign(group, formValue);
        group.type = this.groupTypes.find(t => t.id === group.type_id);

        return group;
    }

    newGroupType() {
        this.dialog.open(DeviceItemGroupTypeDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((type?: DIG_Type) => {});
    }
}
