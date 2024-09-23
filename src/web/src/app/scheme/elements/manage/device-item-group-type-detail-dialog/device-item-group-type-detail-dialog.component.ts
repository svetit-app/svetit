import { Component, inject } from '@angular/core';
import { MatDialogRef, MatDialogTitle, MatDialogContent } from '@angular/material/dialog';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import {DIG_Type} from '../../../scheme';
import { CdkScrollable } from '@angular/cdk/scrolling';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-device-item-group-type-detail-dialog',
    templateUrl: './device-item-group-type-detail-dialog.component.html',
    styleUrls: ['./device-item-group-type-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [MatDialogTitle, ReactiveFormsModule, CdkScrollable, MatDialogContent, MatFormField, MatLabel, MatInput, MatButton]
})
export class DeviceItemGroupTypeDetailDialogComponent extends DetailDialog<DIG_Type, DeviceItemGroupTypeDetailDialogComponent> {
    fg: UntypedFormGroup;

    constructor() {
        const dialogRef = inject<MatDialogRef<DeviceItemGroupTypeDetailDialogComponent>>(MatDialogRef);
        const schemeService = inject(SchemeService);
        const fb = inject(UntypedFormBuilder);

        super(dialogRef, null, schemeService, Structure_Type.ST_DIG_TYPE, fb);
    }

    createFormGroup(): UntypedFormGroup {
        return this.fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
            title: ['', []],
            description: ['', []],
        });
    }
}
