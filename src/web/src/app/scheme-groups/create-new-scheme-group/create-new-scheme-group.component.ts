import { Component, EventEmitter, Input, OnChanges, OnInit, Output, SimpleChanges, inject } from '@angular/core';
import { UntypedFormBuilder, UntypedFormGroup, Validators, ReactiveFormsModule } from '@angular/forms';
import {SchemesService} from '../../schemes/schemes.service';
import {Scheme_Group} from '../../user';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatInput } from '@angular/material/input';

import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-create-new-scheme-group',
    templateUrl: './create-new-scheme-group.component.html',
    styleUrls: ['./create-new-scheme-group.component.css'],
    standalone: true,
    imports: [ReactiveFormsModule, MatFormField, MatLabel, MatInput, MatButton]
})
export class CreateNewSchemeGroupComponent implements OnInit, OnChanges {
    private schemesService = inject(SchemesService);

    @Input() group: Scheme_Group;
    @Output() groupChanged: EventEmitter<Scheme_Group> = new EventEmitter();

    fg: UntypedFormGroup;

    constructor() {
        const fb = inject(UntypedFormBuilder);

        this.fg = fb.group({
            id: [null, []],
            name: ['', [Validators.required]],
        });
    }

    ngOnInit(): void {
    }

    ngOnChanges(changes: SimpleChanges) {
        if (changes.group) {
            if (changes.group.currentValue) {
                this.fg.setValue(changes.group.currentValue);
            } else {
                this.fg.reset();
            }
        }
    }

    submit() {
        if (this.fg.invalid) return;

        if (this.fg.value.id) {
            this.schemesService.updateSchemeGroup(this.fg.value)
                .subscribe((group) => this.groupChanged.emit(group));
        } else {
            this.schemesService.createSchemeGroup(this.fg.value)
                .subscribe((group) => this.groupChanged.emit(group));
        }
    }

    remove() {
        this.schemesService.removeSchemeGroup(this.group.id)
            .subscribe(() => null);
    }
}
