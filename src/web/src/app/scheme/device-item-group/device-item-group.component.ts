import { Component, EventEmitter, Input, OnInit, Output, inject } from '@angular/core';
import {Device_Item, Device_Item_Group, DIG_Mode_Type} from '../scheme';
import {Structure_Type} from '../settings/settings';
import {
    Device_Item_Details,
    DeviceItemDetailDialogComponent
} from '../elements/manage/device-item-detail-dialog/device-item-detail-dialog.component';
import {
    Device_Item_Group_Details,
    DeviceItemGroupDetailDialogComponent
} from '../elements/manage/device-item-group-detail-dialog/device-item-group-detail-dialog.component';
import {ParamsDialogComponent} from './params-dialog/params-dialog.component';
import {filter} from 'rxjs/operators';
import {MatDialog} from '@angular/material/dialog';
import {SchemeService} from '../scheme.service';
import {UIService} from '../../ui.service';
import {ControlService} from '../control.service';
import {AuthService} from '../../auth/service';
import {MediaMatcher} from '@angular/cdk/layout';
import { NgStyle } from '@angular/common';
import { GroupStatusComponent } from '../group-status/group-status.component';
import { MatFormField } from '@angular/material/form-field';
import { MatSelect } from '@angular/material/select';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { MatOption } from '@angular/material/core';
import { MatIconButton, MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { DevItemValueComponent } from '../dev-item-value/dev-item-value.component';

@Component({
    selector: 'app-device-item-group',
    templateUrl: './device-item-group.component.html',
    styleUrls: ['./device-item-group.component.css', '../elements/manage/manage.component.css'],
    standalone: true,
    imports: [NgStyle, GroupStatusComponent, MatFormField, MatSelect, ReactiveFormsModule, FormsModule, MatOption, MatIconButton, MatIcon, DevItemValueComponent, MatButton]
})
export class DeviceItemGroupComponent implements OnInit {
    private dialog = inject(MatDialog);
    private schemeService = inject(SchemeService);
    private ui = inject(UIService);
    private media = inject(MediaMatcher);
    private controlService = inject(ControlService);
    private auth = inject(AuthService);

    @Input() group: Device_Item_Group;
    @Input() isEditorModeEnabled: boolean;

    groupModes: DIG_Mode_Type[];
    canChangeMode: boolean;

    ngOnInit(): void {
        this.groupModes = this.schemeService.scheme.dig_mode_type;
        this.canChangeMode = this.auth.canChangeMode();
    }

    removeGroup(group: Device_Item_Group) {
        this.ui.confirmationDialog()
            .subscribe((confirm) => {
                if (!confirm) {
                    return;
                }

                this.schemeService.remove_structure(Structure_Type.ST_DEVICE_ITEM_GROUP, group)
                    .subscribe(() => {});
            });
    }

    removeItem(item: Device_Item) {
        this.ui.confirmationDialog()
            .subscribe((confirm) => {
                if (!confirm) {
                    return;
                }

                this.schemeService.remove_structure(Structure_Type.ST_DEVICE_ITEM,  item)
                    .subscribe(() => {});
            });
    }

    getDialogWidth(): string {
        return this.media.matchMedia('(max-width: 600px)').matches ? '100%' : '80%';
    }

    newItem(parentGroup: Device_Item_Group) {
        this.dialog
            .open(DeviceItemDetailDialogComponent, {width: this.getDialogWidth(), data: { group_id: parentGroup.id, disableChangeGroupId: true }})
            .afterClosed()
            .subscribe((itemDetails?: Device_Item_Details) => {});
    }

    editItem(item: Device_Item) {
        this.dialog
            .open(DeviceItemDetailDialogComponent, {width: this.getDialogWidth(), data: { ...item, disableChangeGroupId: true }})
            .afterClosed()
            .subscribe((itemDetails?: Device_Item_Details) => {});
    }

    editGroup(group: Device_Item_Group) {
        this.dialog
            .open(DeviceItemGroupDetailDialogComponent, {width: this.getDialogWidth(), data: group})
            .afterClosed()
            .subscribe((groupDetails?: Device_Item_Group_Details) => {});
    }

    openParamsDialog(groupId) {
        this.dialog.open(ParamsDialogComponent, {
            panelClass: 'dig-param-dialog',
            width: this.getDialogWidth(),
            data: {groupId: groupId, isEditorModeEnabled: this.isEditorModeEnabled},
        })
            .afterClosed()
            .pipe(filter(name => name))
            .subscribe(res => {
            });
    }

    changeDIGMode(mode_id: any, group_id: number): void {
        this.controlService.changeGroupMode(mode_id, group_id);
    }
}
