import {Component, forwardRef} from '@angular/core';
import { ControlValueAccessor, UntypedFormBuilder, UntypedFormGroup, NG_VALUE_ACCESSOR, Validators, ReactiveFormsModule } from '@angular/forms';
import {DIG_Param_Type, DIG_Param_Value_Type, DIG_Type} from '../../../scheme';
import {SchemeService} from '../../../scheme.service';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';
import { MatSelect } from '@angular/material/select';

import { MatOption } from '@angular/material/core';

@Component({
    selector: 'app-param-type-form',
    templateUrl: './param-type-form.component.html',
    styleUrls: ['./param-type-form.component.css'],
    providers: [{
            provide: NG_VALUE_ACCESSOR,
            useExisting: forwardRef(() => ParamTypeFormComponent),
            multi: true,
        }],
    standalone: true,
    imports: [
    ReactiveFormsModule,
    MatFormField,
    MatLabel,
    MatInput,
    MatSelect,
    MatOption
],
})
export class ParamTypeFormComponent implements ControlValueAccessor {
    DIG_Param_Value_Type = Object.keys(DIG_Param_Value_Type)
        .map(key => ({ key, value: DIG_Param_Value_Type[key] }))
        .filter(i => typeof i.value === 'number');

    paramTypeFg: UntypedFormGroup;
    groupTypes: DIG_Type[];
    params: DIG_Param_Type[];

    private onChange: any;
    private onTouched: any;

    constructor(scheme: SchemeService, formBuilder: UntypedFormBuilder) {
        this.paramTypeFg = formBuilder.group({
            title: ['', []],
            name: ['', [Validators.required]],
            description: ['', []],
            value_type: [null, [Validators.required]],

            parent_id: [null, []],
            group_type_id: [null, [Validators.required]],
        });

        this.params = scheme.scheme.dig_param_type;
        this.groupTypes = scheme.scheme.dig_type;

        this.paramTypeFg.valueChanges.subscribe(v => {
            this.onTouched && this.onTouched();

            if (this.paramTypeFg.valid) {
                this.onChange && this.onChange(v);
            }
        });
    }

    registerOnChange(fn: any): void {
        this.onChange = fn;
    }

    registerOnTouched(fn: any): void {
        this.onTouched = fn;
    }

    writeValue(obj: any): void {
        if (typeof obj === 'object') {
            this.paramTypeFg.patchValue(obj);
        }
    }
}
