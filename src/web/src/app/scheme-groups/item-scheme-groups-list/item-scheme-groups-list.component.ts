import {Component, EventEmitter, Input, OnChanges, OnInit, Output, SimpleChanges} from '@angular/core';
import {Group_User_Roles, Scheme_Group} from '../../user';
import {UntypedFormBuilder, UntypedFormGroup, Validators} from '@angular/forms';
import {SchemesService} from '../../schemes/schemes.service';

@Component({
    selector: 'app-item-scheme-groups-list',
    templateUrl: './item-scheme-groups-list.component.html',
    styleUrls: ['./item-scheme-groups-list.component.css']
})
export class ItemSchemeGroupsListComponent implements OnInit, OnChanges {
    readonly Group_User_Roles = Group_User_Roles;

    @Input() addWithRole = false;
    @Input() items: Scheme_Group[];
    @Output() add: EventEmitter<Scheme_Group & { role?: Group_User_Roles }> = new EventEmitter();
    @Output() remove: EventEmitter<Scheme_Group> = new EventEmitter();

    addToSchemeGroupFg: UntypedFormGroup;
    schemeGroups: Scheme_Group[];
    private _schemeGroups: Scheme_Group[];

    constructor(
        private schemesService: SchemesService,
        fb: UntypedFormBuilder,
    ) {
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
