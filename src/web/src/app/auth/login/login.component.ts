import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';
import {CookieService} from 'ngx-cookie-service';

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
		private sanitizer: DomSanitizer,
		private cookie: CookieService
	) { }

	ngOnInit() {
		const force = !!this.route.snapshot.data?.complete;
		if (!force)
		{
			this.auth.CheckAndLogout();
			return;
		}

		const token = this.cookie.get("session");
		this.auth.SaveToken(token).subscribe(() => {
			this.cookie.delete("session");

			const redirectPath = this.route.snapshot.queryParams['redirectPath'] || '/dashboard';
			this.router.navigate([redirectPath]);
		});
	}
}
