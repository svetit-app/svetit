import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';
import {CookieService} from 'ngx-cookie-service';

import {AuthService} from '../service';
import { Subscription } from 'rxjs';

@Component({
	selector: 'app-login',
	templateUrl: './login.component.html',
	styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {
	private _sub: Subscription;

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
			this.auth.GoToLogin();
			return;
		}

		const token = this.cookie.get("session");
		this._sub = this.auth.FetchUser().subscribe(res => {
			this._sub.unsubscribe();
			this._sub = null;

			if (!res)
				return;

			const redirectPath = this.route.snapshot.queryParams['redirectPath'] || '/dashboard';
			this.router.navigate([redirectPath]);
		});
	}

	ngOnDestroy() {
		if (this._sub)
			this._sub.unsubscribe();
	}
}
