import {Component, Inject} from '@angular/core';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import { MAT_DIALOG_DATA, MatDialog, MatDialogRef, MatDialogTitle, MatDialogContent } from '@angular/material/dialog';
import {Device_Item_Type, DIG_Type, Register_Type, Save_Algorithm, Save_Timer, Sign_Type} from '../../../scheme';
import {SchemeService} from '../../../scheme.service';
import {SettingsService} from '../../../settings.service';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import {SignTypeDetailDialogComponent} from '../sign-type-detail-dialog/sign-type-detail-dialog.component';
import {DeviceItemGroupTypeDetailDialogComponent} from '../device-item-group-type-detail-dialog/device-item-group-type-detail-dialog.component';
import { CdkScrollable } from '@angular/cdk/scrolling';
import { MatFormField, MatLabel, MatHint } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatSelect } from '@angular/material/select';
import { NgFor, NgIf } from '@angular/common';
import { MatOption } from '@angular/material/core';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-device-item-type-detail-dialog',
    templateUrl: './device-item-type-detail-dialog.component.html',
    styleUrls: ['./device-item-type-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [MatDialogTitle, ReactiveFormsModule, CdkScrollable, MatDialogContent, MatFormField, MatLabel, MatInput, MatSelect, NgFor, MatOption, NgIf, MatHint, MatButton]
})
export class DeviceItemTypeDetailDialogComponent extends DetailDialog<Device_Item_Type, DeviceItemTypeDetailDialogComponent> {
    disableGroupTypeChanging: boolean;

    signTypes: Sign_Type[];
    registerTypes;
    saveAlgos;
    saveTimers: Save_Timer[];
    Save_Algorithm = Save_Algorithm;
    groupTypes: DIG_Type[];

    constructor(
        fb: UntypedFormBuilder,
        dialogRef: MatDialogRef<DeviceItemTypeDetailDialogComponent>,
        @Inject(MAT_DIALOG_DATA) data: Device_Item_Type & { disableGroupTypeChanging: boolean },
        schemeService: SchemeService,
        settings: SettingsService,
        private dialog: MatDialog,
    ) {
        super(dialogRef, data, schemeService, Structure_Type.ST_DEVICE_ITEM_TYPE, fb);
        this.disableGroupTypeChanging = data.disableGroupTypeChanging;

        this.registerTypes = this.enumToArray(Register_Type);
        this.saveAlgos = this.enumToArray(Save_Algorithm);

        this.signTypes = this.schemeService.scheme.sign_type;
        this.groupTypes = this.schemeService.scheme.dig_type;

        settings.getSaveTimers().subscribe(timers => this.saveTimers = timers.results);
    }

    createFormGroup(): UntypedFormGroup {
        const group = this.fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
            title: ['', [Validators.required]],
            group_type_id: [null, [Validators.required]],
            sign_id: [0, []],
            register_type: [null, [Validators.required]],
            save_algorithm: [null, [Validators.required]],
            save_timer_id: [null, []],
        });

        group.controls.save_algorithm.valueChanges.subscribe((v) => {
            const control = group.controls.save_timer_id;
            if (
                v === Save_Algorithm.SA_BY_TIMER
                || v === Save_Algorithm.SA_BY_TIMER_ANY_CASE
            ) {
                control.setValidators([Validators.required]);
            } else {
                control.clearValidators();
            }
        });

        return group;
    }

    patchValue(dialogData) {
        if (dialogData) {
            this.fg.patchValue({
                ...dialogData,
                sign_id: dialogData.sign_id || 0,
            });
        }
    }

    enumToArray(en: any) {
        return Object.keys(en)
            .filter(key => key.length > 2) // filter out numbers
            .map(key => ({ id: en[key], name: key }));
    }

    newSignType() {
        this.dialog.open(SignTypeDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((signType?: Sign_Type) => {});
    }

    newGroupType() {
        this.dialog.open(DeviceItemGroupTypeDetailDialogComponent, { width: '80%' })
            .afterClosed()
            .subscribe((type?: DIG_Type) => {});
    }
}
