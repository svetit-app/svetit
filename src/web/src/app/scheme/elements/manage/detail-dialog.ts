import {MatDialog, MatDialogRef} from '@angular/material/dialog';
import {UntypedFormBuilder, UntypedFormGroup} from '@angular/forms';
import {Modify_Structure_Type, Patch_Structure_Response, SchemeService} from '../../scheme.service';
import {Structure_Type} from '../../settings/settings';
import {SettingsService} from '../../settings.service';
import {Plugin_Type} from '../../scheme';

export abstract class DetailDialog<T extends Modify_Structure_Type,C> {
    public fg: UntypedFormGroup;

    protected constructor(
        protected dialogRef: MatDialogRef<C>,
        protected dialogData: T,
        protected schemeService: SchemeService,
        protected settingName: Structure_Type,
        protected fb: UntypedFormBuilder,
        initFg = true,
    ) {
        if (initFg) {
            this.initFg(dialogData);
        }
    }

    patchValue(dialogData) {
        if (dialogData) {
            this.fg.patchValue(dialogData);
        }
    }

    submit() {
        if (this.fg.invalid) return;

        let prev = this.dialogData?.id ? this.dialogData : null;
        this.schemeService.upsert_structure(
            this.settingName,
            this.createItem(this.fg.value),
            prev,
        )
            .subscribe((r: Patch_Structure_Response<T>) => {
                this.dialogRef.close({
                    ...this.fg.value,
                    id: prev?.id || r.inserted[0].id,
                });
            });
    }

    cancel() {
        this.dialogRef.close(null);
    }

    abstract createFormGroup(): UntypedFormGroup;

    createItem(formValue: any): T {
        return {...formValue};
    }

    protected initFg(dialogData: T) {
        this.fg = this.createFormGroup();
        this.patchValue(dialogData);
    }
}
