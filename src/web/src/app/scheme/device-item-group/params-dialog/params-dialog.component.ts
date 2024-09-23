import {Component, Inject, OnInit} from '@angular/core';
import {Device_Item_Group, DIG_Param, Section} from '../../scheme';
import {Subscription} from 'rxjs';
import {ActivatedRoute} from '@angular/router';
import {SchemeService} from '../../scheme.service';
import {AuthService} from '../../../auth/service';
import {ControlService} from '../../control.service';
import { Location } from '@angular/common';
import {MAT_DIALOG_DATA, MatDialogRef} from '@angular/material/dialog';
import {MatSnackBar} from '@angular/material/snack-bar';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { ParamItemComponent } from '../../param-item/param-item.component';
import { MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { MatProgressSpinner } from '@angular/material/progress-spinner';

@Component({
    selector: 'app-params-dialog',
    templateUrl: './params-dialog.component.html',
    styleUrls: ['../../elements/manage/manage.component.css'],
    standalone: true,
    imports: [
    ReactiveFormsModule,
    FormsModule,
    ParamItemComponent,
    MatButton,
    MatIcon,
    MatProgressSpinner
],
})
export class ParamsDialogComponent implements OnInit {
    pending = false;

    groupId: number;

    sct: Section;
    group: Device_Item_Group = undefined;
    isEditorModeEnabled: boolean;
    cantChange: boolean;

    changed_values: DIG_Param[] = [];
    private group_param_values_changed$: Subscription;
    private changing_timeout: number;
    private unsaved_params_ids: number[];

    constructor(
        private route: ActivatedRoute,
        public schemeService: SchemeService,
        private user: AuthService,
        private controlService: ControlService,
        private location: Location,
        public dialogRef: MatDialogRef<ParamsDialogComponent>,
        private snackBar: MatSnackBar,
        @Inject(MAT_DIALOG_DATA) public data: { groupId: number, isEditorModeEnabled: boolean },
    ) {
        this.groupId = data.groupId;
        this.isEditorModeEnabled = data.isEditorModeEnabled;
    }

    ngOnInit() {
        this.getGroup();
        this.cantChange = !this.user.canChangeParam();
    }

    getGroup(): void {
        const groupId = this.groupId;
        for (const sct of this.schemeService.scheme.section) {
            for (const group of sct.groups) {
                if (group.id === groupId) {
                    this.sct = sct;
                    this.group = group;
                    return;
                }
            }
        }
    }

    onEnter(e: any): void {
        e.preventDefault();
        let control: any;
        control = e.srcElement.parentElement;
        control = control.parentElement;
        control = control.parentElement;
        control = control.parentElement;
        if (control.nextElementSibling) {
            control = control.nextElementSibling;
        } else {
            control = control.parentElement;
            control = control.parentElement;
            control = control.nextElementSibling;
        }

        let findNode = (control: any): boolean => {
            if (!control) {
                return;
            }
            if ((!control.hidden) &&
                (control.nodeName == 'INPUT' ||
                    control.nodeName == 'SELECT' ||
                    control.nodeName == 'BUTTON' ||
                    control.nodeName == 'TEXTAREA')) {
                control.focus();
                return true;
            }

            for (const node of control.childNodes) {
                if (findNode(node)) {
                    return true;
                }
            }
            return false;
        };

        findNode(control);
    }

    onSubmit() {
        if (!this.schemeService.isSchemeConnected) {
            return;
        }

        console.log('param form submit', this.changed_values);
        if (this.changed_values?.length > 0) {
            this.pending = true;
            this.group_param_values_changed$ = this.controlService.group_param_values_changed.subscribe((changed_params: DIG_Param[]) => {
                if (!this.unsaved_params_ids) {
                    this.unsaved_params_ids = this.changed_values.map(p => p.id);
                }

                this.clear_changing_timeout();
                this.unsaved_params_ids = this.unsaved_params_ids.filter(id => !changed_params.find(p => p.id === id));

                if (this.unsaved_params_ids.length === 0) {
                    this.pending = false;
                    this.close();
                }
            });
            this.set_changing_timeout();
            this.controlService.changeParamValues(this.changed_values);
        }
    }

    goBack(): void {
        console.log('BACK!');
        this.location.back();
    }

    close() {
        this.dialogRef.close();
    }

    private set_changing_timeout() {
        this.changing_timeout = window.setTimeout(() => {
            this.group_param_values_changed$.unsubscribe();

            if (this.unsaved_params_ids?.length > 0) {
                this.changing_error_unchanged();
            } else {
                this.changing_error_timeout();
            }
        }, 10000);
    }

    private clear_changing_timeout() {
        clearTimeout(this.changing_timeout);
    }

    private changing_error_unchanged() {
        this.pending = false;

        this.snackBar.open('Не все значения изменены', 'Скрыть', {
            duration: 10000,
            horizontalPosition: 'center',
            verticalPosition: 'bottom',
        });
    }

    private changing_error_timeout() {
        this.pending = false;

        this.snackBar.open('Тайм-аут обновления', 'Скрыть', {
            duration: 10000,
            horizontalPosition: 'center',
            verticalPosition: 'bottom',
        });
    }
}
