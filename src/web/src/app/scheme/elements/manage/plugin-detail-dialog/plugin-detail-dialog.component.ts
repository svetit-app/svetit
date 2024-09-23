import {Component, Inject} from '@angular/core';
import {DetailDialog} from '../detail-dialog';
import {Plugin_Type} from '../../../scheme';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {MAT_DIALOG_DATA, MatDialogRef} from '@angular/material/dialog';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-plugin-detail-dialog',
    templateUrl: './plugin-detail-dialog.component.html',
    styleUrls: ['./plugin-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [ReactiveFormsModule, MatFormField, MatLabel, MatInput, MatButton]
})
export class PluginDetailDialogComponent extends DetailDialog<Plugin_Type, PluginDetailDialogComponent> {

    constructor(
        fb: UntypedFormBuilder,
        dialogRef: MatDialogRef<PluginDetailDialogComponent>,
        @Inject(MAT_DIALOG_DATA) data: Plugin_Type,
        schemeService: SchemeService,
    ) {
        super(dialogRef, data, schemeService, Structure_Type.ST_PLUGIN_TYPE, fb);
    }

    ngOnInit(): void {
    }

    createFormGroup(): UntypedFormGroup {
        return this.fb.group({
            'id': [],
            'name': ['', [Validators.required]],
        });
    }

}
