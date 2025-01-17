import { Component, inject } from '@angular/core';
import { MatDialogRef, MatDialogTitle, MatDialogContent } from '@angular/material/dialog';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import { CdkScrollable } from '@angular/cdk/scrolling';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-sign-type-detail-dialog',
    templateUrl: './sign-type-detail-dialog.component.html',
    styleUrls: ['./sign-type-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [MatDialogTitle, ReactiveFormsModule, CdkScrollable, MatDialogContent, MatFormField, MatLabel, MatInput, MatButton]
})
export class SignTypeDetailDialogComponent {
    private dialogRef = inject<MatDialogRef<SignTypeDetailDialogComponent>>(MatDialogRef);
    private schemeService = inject(SchemeService);

    fg: UntypedFormGroup;

    constructor() {
        const fb = inject(UntypedFormBuilder);

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
