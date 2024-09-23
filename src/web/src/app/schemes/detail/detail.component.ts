import {Component, OnInit} from '@angular/core';
import { ActivatedRoute, RouterLink } from '@angular/router';
import { Location, NgIf, NgFor, UpperCasePipe } from '@angular/common';

import {Scheme, Scheme_Group} from '../../user';
import {SchemesService} from '../schemes.service';
import {AuthService} from '../../auth/service';
import { MatIcon } from '@angular/material/icon';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { MatFormField, MatLabel } from '@angular/material/form-field';
import { MatSelect } from '@angular/material/select';
import { MatOption } from '@angular/material/core';
import { MatButton } from '@angular/material/button';
import { ItemSchemeGroupsListComponent } from '../../scheme-groups/item-scheme-groups-list/item-scheme-groups-list.component';

@Component({
    selector: 'app-scheme-detail',
    templateUrl: './detail.component.html',
    styleUrls: ['./detail.component.css'],
    standalone: true,
    imports: [NgIf, MatIcon, ReactiveFormsModule, FormsModule, MatFormField, MatLabel, MatSelect, MatOption, NgFor, MatButton, ItemSchemeGroupsListComponent, RouterLink, UpperCasePipe]
})
export class SchemeDetailComponent implements OnInit {
    scheme: Scheme;
    can_save: boolean;
    canChangeName: boolean;
    nameEditing: boolean = false;
    cities: any[];
    comps: any[];
    schemeGroups: Scheme_Group[];

    constructor(
        private route: ActivatedRoute,
        private schemesService: SchemesService,
        private auth: AuthService,
        private location: Location,
    ) {
    }

    ngOnInit() {
        this.can_save = true;
        this.canChangeName = this.auth.checkPermission('add_scheme');
        this.getScheme();

        this.schemesService.getCities().subscribe(data => {
            this.cities = data.results;
        });

        this.schemesService.getCompanies().subscribe(data => {
            this.comps = data.results;
        });
    }

    getScheme(): void {
        const name = this.getSchemeName();
        this.schemesService.getScheme(name)
            .subscribe(scheme => {
                this.scheme = scheme;

                this.fetchGroupsForScheme();
            });
    }

    save(): void {
        if (this.scheme.city == 0) {
            this.scheme.city = null;
        }

        if (this.scheme.company == 0) {
            this.scheme.company = null;
        }

        this.schemesService.updateScheme(this.scheme)
            .subscribe(() => null);
    }

    goBack(): void {
        this.location.back();
    }

    changeName(name_edit: any): void {
        if (!this.canChangeName) {
            return;
        }
        this.nameEditing = true;

        name_edit.style.display = '';
        name_edit.focus();
    }

    saveName(name: string): void {
        this.nameEditing = false;
        this.schemesService.setName(this.scheme.id, name).subscribe(() => {
            this.scheme.name = name;
        });
    }

    cancel_name_edit(): void {
        this.nameEditing = false;
    }

    private getSchemeName(): string {
        const p_str: string = 'name';
        if (this.route.snapshot.paramMap.has(p_str)) {
            return this.route.snapshot.paramMap.get(p_str);
        }
        return this.route.parent.snapshot.paramMap.get(p_str);
    }

    removeFromSchemeGroup(element: Scheme_Group) {
        this.schemesService.removeSchemeFromSchemeGroup(element.id, this.scheme.id)
            .subscribe(() => this.fetchGroupsForScheme());
    }

    addToSchemeGroup($event: Scheme_Group) {
        this.schemesService.addSchemeToSchemeGroup($event.id, this.scheme.id)
            .subscribe(() => this.fetchGroupsForScheme());
    }

    private fetchGroupsForScheme() {
        this.schemesService.getSchemeGroupsForScheme(this.scheme.id)
            .subscribe(groups => this.schemeGroups = groups);
    }
}
