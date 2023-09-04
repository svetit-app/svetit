import {Component, OnInit} from '@angular/core';
import {Group_User_Roles, Scheme_Group} from '../../user';
import {SchemesService} from '../../schemes/schemes.service';
import {UserService} from '../../user/service';

@Component({
	selector: 'app-scheme-groups',
	templateUrl: './scheme-groups.component.html',
	styleUrls: ['./scheme-groups.component.css']
})
export class SchemeGroupsComponent implements OnInit {
	userSchemeGroups: Scheme_Group[];

	constructor(private schemes: SchemesService, private user: UserService) {
	}

	ngOnInit(): void {
		this.fetchGroups();
	}

	addUserToSchemeGroup($event: Scheme_Group & { role?: Group_User_Roles }) {
		this.schemes.addUserToSchemeGroup($event.id, this.user.info.id, $event.role)
			.subscribe(() => this.fetchGroups());
	}

	removeUserFromSchemeGroup($event: Scheme_Group) {
		this.schemes.removeUserFromSchemeGroup($event.id, this.user.info.id)
			.subscribe(() => this.fetchGroups());
	}

	private fetchGroups() {
		this.schemes.getSchemeGroupsForUser(this.user.info.id)
			.subscribe(groups => this.userSchemeGroups = groups);
	}
}
