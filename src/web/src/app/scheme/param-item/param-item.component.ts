import {Component, Input, OnChanges, SimpleChanges} from '@angular/core';

import {SchemeService} from '../scheme.service';
import {DIG_Param, DIG_Param_Type, DIG_Param_Value_Type} from '../scheme';
import {UntypedFormControl, Validators} from '@angular/forms';
import {Structure_Type} from '../settings/settings';
import {UIService} from '../../ui.service';
import {tap} from 'rxjs/operators';
import {MatDialogRef} from '@angular/material/dialog';
import {addParamsToGroups} from '../add-params-helpers';

@Component({
    selector: 'app-param-item',
    templateUrl: './param-item.component.html',
    styleUrls: ['./param-item.component.css']
})
export class ParamItemComponent implements OnChanges {
    @Input() groupTypeId: number;
    @Input() groupId: number;
    @Input() values: DIG_Param[] = [];
    @Input() changed: DIG_Param[];
    @Input() parent_param: DIG_Param_Type = null;
    @Input() editorModeEnabled = false;

    value_type = DIG_Param_Value_Type;
    params: DIG_Param_Type[];

    showForm = false;
    showNestedParamTypeForm = false;

    paramTypeIdFormControl: UntypedFormControl;
    paramTypeFormControl: UntypedFormControl;
    currentEditingParam: DIG_Param;
    addParamsToGroups: boolean;

    constructor(
        private schemeService: SchemeService,
        private ui: UIService,
        private dialogRef: MatDialogRef<ParamItemComponent>,
    ) {
        this.paramTypeFormControl = new UntypedFormControl(null, []);
        this.paramTypeIdFormControl = new UntypedFormControl(null, [Validators.required]);

        this.paramTypeIdFormControl.valueChanges.subscribe((v) => {
            this.showNestedParamTypeForm = v === 'new';

            if (this.showNestedParamTypeForm) {
                this.paramTypeFormControl.setValidators([Validators.required]);
            } else {
                this.paramTypeFormControl.clearValidators();
            }
        });
    }

    ngOnChanges(changes: SimpleChanges) {
        const paramTypePatch: Partial<DIG_Param_Type> = {};

        if (changes.parent_param || changes.groupTypeId) {
            this.getParamTypes();

            if (changes.groupTypeId) {
                paramTypePatch.group_type_id = changes.groupTypeId.currentValue;
            }

            if (changes.parent_param) {
                paramTypePatch.parent_id = changes.parent_param.currentValue.id;
            }
        }

        if (Object.keys(paramTypePatch).length > 0) {
            this.paramTypeFormControl.setValue(paramTypePatch);
        }
    }

    isDisabled(p: DIG_Param): boolean {
        return this.schemeService.scheme.disabled_param.includes(p.param_id);
    }

    getTimeString(p: DIG_Param): string {
        let pad = (val: number) => {
            return ('0' + val.toFixed(0)).slice(-2);
        };
        let secs = parseInt(p.value);
        let h = pad(Math.floor(secs / 3600));
        secs %= 3600;
        let m = pad(Math.floor(secs / 60));
        return h + ':' + m + ':' + pad(secs % 60);
    }

    prepareValue(p: DIG_Param_Value_Type, val: string): string {
        if (p === DIG_Param_Value_Type.VT_TIME) {
            let arr = val.split(':');
            if (arr.length) {
                let v = parseInt(arr[0]) * 3600;
                let new_value = !Number.isNaN(v) ? v : 0;

                if (arr.length > 1) {
                    v = parseInt(arr[1]) * 60;
                }
                new_value += !Number.isNaN(v) ? v : 0;

                if (arr.length > 2) {
                    v = parseInt(arr[2]);
                }
                new_value += !Number.isNaN(v) ? v : 0;

                return new_value.toString();
            }
        }

        return val;
    }

    change(item: DIG_Param, new_value: any): void {
        for (let idx in this.changed) {
            const param_value = this.changed[idx];
            if (param_value.id === item.id) {
                const new_prepared_value = this.prepareValue(param_value.param.value_type, new_value);

                if (param_value.value != new_prepared_value) {
                    param_value.value = new_prepared_value;
                }

                if (new_prepared_value == item.value) {
                    this.changed.splice(+idx, 1);
                }
                return;
            }
        }

        const copy = {...item};
        const new_prepared_value = this.prepareValue(item.param.value_type, new_value);
        if (copy.value != new_prepared_value) {
            copy.value = new_prepared_value;
        }

        this.changed.push(copy);
    }

    addParamForm() {
        this.resetForm();

        this.showForm = true;
    }

    editParamForm(param: DIG_Param) {
        this.resetForm();

        this.showForm = true;
        this.currentEditingParam = param;
        this.paramTypeIdFormControl.setValue(param.param_id);
    }

    removeParam(param: DIG_Param) {
        this.ui.confirmationDialog()
            .subscribe((result) => {
                if (!result) return;

                this.schemeService.remove_structure(Structure_Type.ST_DIG_PARAM, param)
                    .subscribe(() => {});
            });
    }

    submitForm() {
        if (this.showNestedParamTypeForm) {
            this.createParamType().subscribe((paramTypeResponse) => {
                let of;
                if (this.addParamsToGroups) {
                    of = addParamsToGroups(this.schemeService, this.groupTypeId, paramTypeResponse.inserted.map(i => i.id));
                } else {
                    of = this.createParam(paramTypeResponse.inserted[0].id);
                }

                of.subscribe(() => {
                    this.resetForm();
                });
            });
        } else {
            if (this.paramTypeIdFormControl.valid) {
                const paramId = this.paramTypeIdFormControl.value;
                this.createParam(paramId)
                    .subscribe((response) => {
                        this.resetForm();
                    });
            }
        }
    }

    createParamType() {
        const paramType: Omit<DIG_Param_Type, 'id'> = this.paramTypeFormControl.value;
        return this.schemeService.upsert_structure<DIG_Param_Type>(Structure_Type.ST_DIG_PARAM_TYPE, paramType)
            .pipe(tap(() => this.getParamTypes()));
    }

    createParam(paramTypeId: number) {
        const param: Omit<DIG_Param, 'value' | 'id'> = {
            param_id: paramTypeId,
            group_id: this.groupId,
            param: this.params.find(p => p.id === paramTypeId),
            childs: [],
        };

        return this.schemeService.upsert_structure(
            Structure_Type.ST_DIG_PARAM,
            param as any, // TODO: possible TypeScript bug with nested Omit<>
        );
    }

    resetForm() {
        if (this.showNestedParamTypeForm) {
            this.showNestedParamTypeForm = false;
        }

        this.addParamsToGroups = true;
        this.showForm = false;
        this.currentEditingParam = null;
        this.paramTypeIdFormControl.reset();
    }

    private getParamTypes() {
        let existingParams;
        if (this.parent_param) {
            existingParams = this.parent_param.childs;
        } else {
            for (let s of this.schemeService.scheme.section) {
                for (let g of s.groups) {
                    if (this.groupId === g.id) {
                        existingParams = g.params;
                        break;
                    }
                }

                if (existingParams) {
                    break;
                }
            }
        }

        this.params = this.schemeService.scheme.dig_param_type.filter((param) => {
            if (param.group_type_id !== this.groupTypeId) return false;
            if (existingParams.find(p => p.id === param.id)) return false;

            return (!this.parent_param && param.parent_id === null) || (this.parent_param?.id === param.parent_id);
        });
    }
}
