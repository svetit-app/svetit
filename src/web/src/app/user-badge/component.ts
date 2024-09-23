import { Component, OnInit, Input, inject } from '@angular/core';

import { AuthService } from '../auth/service';
import { UserBadgeService } from './service';

@Component({
    selector: 'app-user-badge',
    templateUrl: './component.html',
    styleUrls: ['./component.css'],
    standalone: true
})
export class UserBadgeComponent implements OnInit {
	userBadges = inject(UserBadgeService);
	auth = inject(AuthService);

	@Input() initialized: boolean;
	@Input() showDropDown: boolean;
	@Input() mobileQueryMatches: boolean;

	ngOnInit() {
	}
}
