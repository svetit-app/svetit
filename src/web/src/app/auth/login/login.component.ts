import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';

import {Tokens} from '../../auth/model';
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
		const token = {
			access: this.route.snapshot.queryParams['access'],
			refresh: this.route.snapshot.queryParams['refresh'],
			logout: this.route.snapshot.queryParams['logout']
		} as Tokens;
		if (!this.auth.SaveTokens(token))
			return;

		const redirectPath = this.route.snapshot.queryParams['redirectPath'] || '/dashboard';
		this.router.navigate([redirectPath]);
	}
}
