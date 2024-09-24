import { Component, OnInit, Input, inject } from '@angular/core';
import { MatIconModule } from '@angular/material/icon';
import {MatBadgeModule} from '@angular/material/badge';

import { AuthService } from '../auth/service';
import { UserBadgeService } from './service';

@Component({
	selector: 'app-user-badge',
	templateUrl: './component.html',
	styleUrls: ['./component.css'],
	standalone: true,
	imports: [
		MatIconModule, MatBadgeModule,
	]
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
