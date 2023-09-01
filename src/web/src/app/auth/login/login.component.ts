import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';

import {Session} from '../../auth/model';
import {AuthService} from '../service';

@Component({
	selector: 'app-login',
	templateUrl: './login.component.html',
	styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

	constructor(
		private route: ActivatedRoute,
		private router: Router,
		private auth: AuthService,
		private sanitizer: DomSanitizer
	) { }

	ngOnInit() {
		const session = {
			token: this.route.snapshot.queryParams['session'],
			username: this.route.snapshot.queryParams['username'],
			userLogin: this.route.snapshot.queryParams['userLogin'],
			userId: this.route.snapshot.queryParams['userId'],
		} as Session;
		if (!this.auth.SaveSession(session))
			return;

		const redirectPath = this.route.snapshot.queryParams['redirectPath'] || '/dashboard';
		this.router.navigate([redirectPath]);
	}
}
