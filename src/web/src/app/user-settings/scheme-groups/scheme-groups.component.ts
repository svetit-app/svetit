import { Component, OnInit, inject } from '@angular/core';
import {Group_User_Roles, Scheme_Group} from '../../user';
import {SchemesService} from '../../schemes/schemes.service';
import {AuthService} from '../../auth/service';

@Component({
    selector: 'app-scheme-groups',
    templateUrl: './scheme-groups.component.html',
    styleUrls: ['./scheme-groups.component.css'],
    standalone: true
})
export class SchemeGroupsComponent implements OnInit {
	private schemes = inject(SchemesService);
	private auth = inject(AuthService);

	userSchemeGroups: Scheme_Group[];

	ngOnInit(): void {
		this.fetchGroups();
	}

	addUserToSchemeGroup($event: Scheme_Group & { role?: Group_User_Roles }) {
		this.schemes.addUserToSchemeGroup($event.id, this.auth.user.id, $event.role)
			.subscribe(() => this.fetchGroups());
	}

	removeUserFromSchemeGroup($event: Scheme_Group) {
		this.schemes.removeUserFromSchemeGroup($event.id, this.auth.user.id)
			.subscribe(() => this.fetchGroups());
	}

	private fetchGroups() {
		this.schemes.getSchemeGroupsForUser(this.auth.user.id)
			.subscribe(groups => this.userSchemeGroups = groups);
	}
}
