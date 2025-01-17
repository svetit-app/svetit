import { Component, EventEmitter, Input, OnChanges, OnInit, Output, SimpleChanges, inject } from '@angular/core';
import {Group_User_Roles, Scheme_Group} from '../../user';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemesService} from '../../schemes/schemes.service';
import { MatTable, MatColumnDef, MatHeaderCellDef, MatHeaderCell, MatCellDef, MatCell, MatFooterCellDef, MatFooterCell, MatHeaderRowDef, MatHeaderRow, MatRowDef, MatRow, MatFooterRowDef, MatFooterRow } from '@angular/material/table';
import { MatIconButton, MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatSelect } from '@angular/material/select';

import { MatOption } from '@angular/material/core';

@Component({
    selector: 'app-item-scheme-groups-list',
    templateUrl: './item-scheme-groups-list.component.html',
    styleUrls: ['./item-scheme-groups-list.component.css'],
    standalone: true,
    imports: [MatTable, MatColumnDef, MatHeaderCellDef, MatHeaderCell, MatCellDef, MatCell, MatIconButton, MatIcon, MatFooterCellDef, MatFooterCell, ReactiveFormsModule, MatFormField, MatLabel, MatSelect, MatOption, MatButton, MatHeaderRowDef, MatHeaderRow, MatRowDef, MatRow, MatFooterRowDef, MatFooterRow]
})
export class ItemSchemeGroupsListComponent implements OnInit, OnChanges {
    private schemesService = inject(SchemesService);

    readonly Group_User_Roles = Group_User_Roles;

    @Input() addWithRole = false;
    @Input() items: Scheme_Group[];
    @Output() add: EventEmitter<Scheme_Group & { role?: Group_User_Roles }> = new EventEmitter();
    @Output() remove: EventEmitter<Scheme_Group> = new EventEmitter();

    addToSchemeGroupFg: UntypedFormGroup;
    schemeGroups: Scheme_Group[];
    private _schemeGroups: Scheme_Group[];

    constructor() {
        const fb = inject(UntypedFormBuilder);

        this.addToSchemeGroupFg = fb.group({
            id: [null, [Validators.required]],
            role: [null, []],
        });

        this.schemesService.get_scheme_groups()
            .subscribe((groups) => this._schemeGroups = groups);
    }

    ngOnInit(): void {
    }

    ngOnChanges(changes: SimpleChanges) {
        if (changes.addWithRole) {
            const { role } = this.addToSchemeGroupFg.controls;

            if (changes.addWithRole.currentValue) {
                role.setValidators([Validators.required]);
            } else {
                role.clearValidators();
            }
        }

        if (changes.items?.currentValue) {
            const items = changes.items.currentValue;
            this.schemeGroups = this._schemeGroups
                    .filter(group => !items.find(i => i.id === group.id));
        }
    }

    submit() {
        if (this.addToSchemeGroupFg.invalid) return;

        this.add.emit(this.addToSchemeGroupFg.value);
    }
}
