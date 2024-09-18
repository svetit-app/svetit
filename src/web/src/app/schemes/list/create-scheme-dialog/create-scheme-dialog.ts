import { Component, OnInit, Inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { AbstractControl, UntypedFormControl, UntypedFormGroup, FormGroupDirective, NgForm, Validators, ValidatorFn, AsyncValidatorFn, ValidationErrors } from '@angular/forms';
import { ErrorStateMatcher } from '@angular/material/core';

import { SubscriptionLike, Observable, of } from 'rxjs';
import { catchError, switchMap, map, delay } from 'rxjs/operators';

import { Scheme, Scheme_Group } from '../../../user';
import { SchemesService } from '../../schemes.service';
import {TranslateService} from '@ngx-translate/core';

import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { MatButtonModule } from '@angular/material/button';
import { MatSelectModule } from '@angular/material/select';
import {
  MatDialog,
  MatDialogActions,
  MatDialogContent,
  MatDialogRef,
  MatDialogTitle,
  MAT_DIALOG_DATA
} from '@angular/material/dialog';
import {AngularMultiSelectModule} from 'angular2-multiselect-dropdown';

import { ReactiveFormsModule } from '@angular/forms';

/** Error when invalid control is dirty, touched, or submitted. */
export class MyErrorStateMatcher implements ErrorStateMatcher {
    isErrorState(control: UntypedFormControl | null, form: FormGroupDirective | NgForm | null): boolean {
        const isSubmitted = form && form.submitted;
        return !!(control && (control.invalid || control.pending) && (control.dirty || control.touched || isSubmitted));
    }
}

export function forbidden_name_validator(nameRe: RegExp): ValidatorFn
{
    return (control: AbstractControl): {[key: string]: any} | null => {
        const is_valid = nameRe.test(control.value);
        return is_valid ? null : {'forbiddenName': {value: control.value}};
    };
}

export function unique_scheme_name_validator(schemesService: SchemesService): AsyncValidatorFn
{
    return (control: AbstractControl): Promise<ValidationErrors | null> | Observable<ValidationErrors | null> =>
    {
        return of(null).pipe(
            delay(300),
            switchMap(val => {
                return schemesService.getScheme(control.value).pipe(
                    map(scheme => scheme ? { uniqueName: true } : null),
                    catchError(() => of(null))
                );
            })
        );
    };
}

@Component({
  selector: 'app-create-scheme-dialog',
	standalone: true,
	imports: [
		CommonModule, ReactiveFormsModule, AngularMultiSelectModule,
		MatSelectModule, MatInputModule, MatFormFieldModule, MatButtonModule,
		MatDialogActions, MatDialogTitle, MatDialogContent,
	],
  templateUrl: './create-scheme-dialog.html',
  styleUrls: [ './create-scheme-dialog.css']
})
export class Create_Scheme_Dialog implements OnInit {
    fc_name = new UntypedFormControl('', [
        Validators.required,
        Validators.minLength(3),
        forbidden_name_validator(/^[a-zA-Z][a-zA-Z0-9\_]+$/)
    ], unique_scheme_name_validator(this.schemesService));

    fc_title = new UntypedFormControl('');
    fc_address = new UntypedFormControl('');
    fc_cities = new UntypedFormControl(null);
    fc_comp = new UntypedFormControl(null);
    fc_desc = new UntypedFormControl('');
    fc_parent = new UntypedFormControl(null);
    fc_s_groups = new UntypedFormControl([], [Validators.required]);

    form = new UntypedFormGroup({
        name: this.fc_name,
        title: this.fc_title,
        address: this.fc_address,
        city_id: this.fc_cities,
        company_id: this.fc_comp,
        description: this.fc_desc,
        parent_id: this.fc_parent,
        scheme_groups: this.fc_s_groups
    });

    matcher = new MyErrorStateMatcher();

    cities: any[];
    comps: any[];
    schemes: any[] = [];

    scheme_groups: Scheme_Group[] = [];
    scheme_group_selected: Scheme_Group[] = [];
    scheme_group_settings = {};

    is_title_gen: boolean = true;

    constructor(
        private schemesService: SchemesService,
        private translate: TranslateService,
        public dialogRef: MatDialogRef<Create_Scheme_Dialog>,
        @Inject(MAT_DIALOG_DATA) public data: any)
    {
        this.cities = data.cities;
        this.comps = data.comps;
    }

    ngOnInit(): void
    {
        this.schemesService.get_parent_schemes().subscribe(schemes => this.schemes = schemes);
        this.schemesService.get_scheme_groups().subscribe(scheme_groups => this.scheme_groups = scheme_groups);

        this.scheme_group_settings = {
            text: this.translate.instant('SELECT_SCHEME_GROUP'),
            selectAllText: this.translate.instant('SELECT_ALL'),
            unSelectAllText: this.translate.instant('DESELECT_ALL'),
            classes: "scheme-groups",
            labelKey: 'name'
        };
    }

    onNoClick(): void
    {
        this.dialogRef.close();
    }

    create(): void
    {
        let scheme = this.form.value;
        scheme.scheme_groups = scheme.scheme_groups.map(sg => sg.id);
        this.dialogRef.close(scheme);
    }

    name_change(): void
    {
        if (this.is_title_gen)
            this.fc_title.setValue(this.fc_name.value);
    }

    title_change(): void
    {
        this.is_title_gen = this.fc_title.value.length === 0;
        this.name_change();
    }
}

