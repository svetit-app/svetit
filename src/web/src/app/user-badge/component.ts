import { Component, OnInit, Input } from '@angular/core';
import { MatIconModule } from '@angular/material/icon';
import { MatBadgeModule } from '@angular/material/badge';

import { AuthService } from '../auth/service';
import { UserBadgeService } from './service';

@Component({
	selector: 'app-user-badge',
	standalone: true,
	imports: [
		MatIconModule,
		MatBadgeModule,
	],
	templateUrl: './component.html',
	styleUrls: ['./component.css']
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
