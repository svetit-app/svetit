import {Component} from '@angular/core';
import {MatLegacyDialogRef as MatDialogRef} from '@angular/material/legacy-dialog';
import {UntypedFormBuilder, UntypedFormGroup, Validators} from '@angular/forms';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';

@Component({
    selector: 'app-sign-type-detail-dialog',
    templateUrl: './sign-type-detail-dialog.component.html',
    styleUrls: ['./sign-type-detail-dialog.component.css', '../detail-dialog.css']
})
export class SignTypeDetailDialogComponent {
    fg: UntypedFormGroup;

    constructor(
        private dialogRef: MatDialogRef<SignTypeDetailDialogComponent>,
        private schemeService: SchemeService,
        fb: UntypedFormBuilder,
    ) {
        this.fg = fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
        });
    }

    submit() {
        if (this.fg.invalid) return;

        this.schemeService.upsert_structure(Structure_Type.ST_SIGN_TYPE, { ...this.fg.value })
            .subscribe(() => {
                this.dialogRef.close(this.fg.value);
            });
    }

    cancel() {
        this.dialogRef.close(null);
    }
}
