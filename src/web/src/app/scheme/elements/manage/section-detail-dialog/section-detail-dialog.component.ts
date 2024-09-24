import { Component, inject } from '@angular/core';
import {Section} from '../../../scheme';
import {MAT_DIALOG_DATA, MatDialogRef} from '@angular/material/dialog';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemeService} from '../../../scheme.service';
import {Structure_Type} from '../../../settings/settings';
import {DetailDialog} from '../detail-dialog';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatButton } from '@angular/material/button';

export type Section_Details = Pick<Section, "name" | "day_start" | "day_end">

@Component({
    selector: 'app-section-detail-dialog',
    templateUrl: './section-detail-dialog.component.html',
    styleUrls: ['./section-detail-dialog.component.css', '../detail-dialog.css'],
    standalone: true,
    imports: [
        ReactiveFormsModule,
        MatFormField,
        MatLabel,
        MatInput,
        MatButton,
    ],
})
export class SectionDetailDialogComponent extends DetailDialog<Section, SectionDetailDialogComponent> {
    private section: Section;


    constructor() {
        const section = inject<Section>(MAT_DIALOG_DATA);
        const dialogRef = inject<MatDialogRef<SectionDetailDialogComponent>>(MatDialogRef);
        const schemeService = inject(SchemeService);
        const fb = inject(UntypedFormBuilder);

        super(dialogRef, section, schemeService, Structure_Type.ST_SECTION, fb);
        this.section = section;

    }

    patchValue(dialogData) {
        if (dialogData) {
            this.fg.patchValue({
                ...dialogData,
                day_start: this.convertSecondsToStringTime(dialogData.day_start),
                day_end: this.convertSecondsToStringTime(dialogData.day_end),
            });
        }
    }

    createItem(formValue: any): Section {
        return {
            ...formValue,
            day_start: SectionDetailDialogComponent.convertTimeStringToSeconds(formValue.day_start),
            day_end: SectionDetailDialogComponent.convertTimeStringToSeconds(formValue.day_end),
            groups: this.section.groups || [],
        };
    }

    createFormGroup(): UntypedFormGroup {
        return this.fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
            day_start: [null, [Validators.required]],
            day_end: [null, [Validators.required]],
        });
    }

    private convertSecondsToStringTime(seconds: number): string {
        const pad = (v: number): string => v < 10 ? `0${v}` : v.toString();
        const d = new Date(seconds * 1000);

        return `${pad(d.getUTCHours())}:${pad(d.getUTCMinutes())}:${pad(d.getUTCSeconds())}`;
    }

    private static convertTimeStringToSeconds(str: string): number {
        const [hours, minutes, seconds] = str.split(':');
        return parseInt(hours, 10) * 3600
            + (parseInt(minutes, 10) || 0) * 60
            + parseInt(seconds, 10) || 0;
    }
}
