import { Component, OnInit, Input } from '@angular/core';

import { AuthService } from '../auth/service';
import { UserBadgeService } from './service';

@Component({
    selector: 'app-user-badge',
    templateUrl: './component.html',
    styleUrls: ['./component.css'],
    standalone: true
})
export class UserBadgeComponent implements OnInit {
	@Input() initialized: boolean;
	@Input() showDropDown: boolean;
	@Input() mobileQueryMatches: boolean;

	constructor(
		public userBadges: UserBadgeService,
		public auth: AuthService,
	) {
	}

	ngOnInit() {
	}
}
