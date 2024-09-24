import { Component, OnInit, inject } from '@angular/core';
import {Scheme_Group} from '../../user';
import {SchemesService} from '../../schemes/schemes.service';
import {ActivatedRoute, NavigationEnd, Router} from '@angular/router';
import {TitleService} from '../../title.service';
import {filter} from 'rxjs/operators';
import { CreateNewSchemeGroupComponent } from '../create-new-scheme-group/create-new-scheme-group.component';

import { UsersAndSchemesComponent } from '../users-and-schemes/users-and-schemes.component';

@Component({
    selector: 'app-edit-scheme-group',
    templateUrl: './edit-scheme-group.component.html',
    styleUrls: ['./edit-scheme-group.component.css'],
    standalone: true,
    imports: [CreateNewSchemeGroupComponent, UsersAndSchemesComponent]
})
export class EditSchemeGroupComponent implements OnInit {
    private schemes = inject(SchemesService);
    private router = inject(Router);
    private titleService = inject(TitleService);

    group: Scheme_Group;

    constructor() {
        const route = inject(ActivatedRoute);

        route.paramMap.subscribe((p) => {
            const id = p.get('id');
            if (id === 'new') {
                this.group = null;
            } else {
                this.schemes.getSchemeGroup(+id)
                    .subscribe(group => {
                        this.group = group;
                        this.titleService.setVariable('%GROUP%', this.group.name);
                    });
            }
        });
    }

    ngOnInit(): void {
    }

    groupChanged($event: Scheme_Group) {
        if ($event.id !== this.group.id) {
            this.router.navigate(['group', $event.id]);
        }
    }
}
