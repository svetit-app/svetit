import {Component, Inject} from '@angular/core';
import {DetailDialog} from '../detail-dialog';
import {Plugin_Type} from '../../../scheme';
import {UntypedFormBuilder, UntypedFormGroup, Validators} from '@angular/forms';
import {MAT_LEGACY_DIALOG_DATA as MAT_DIALOG_DATA, MatLegacyDialogRef as MatDialogRef} from '@angular/material/legacy-dialog';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';

@Component({
    selector: 'app-plugin-detail-dialog',
    templateUrl: './plugin-detail-dialog.component.html',
    styleUrls: ['./plugin-detail-dialog.component.css', '../detail-dialog.css']
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
