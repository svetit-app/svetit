import { Component, OnInit, inject } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';
import {CookieService} from 'ngx-cookie-service';

import {AuthService} from '../service';
import { Subscription } from 'rxjs';

@Component({
    selector: 'app-login',
    templateUrl: './login.component.html',
    styleUrls: ['./login.component.css'],
    standalone: true
})
export class LoginComponent implements OnInit {
	private route = inject(ActivatedRoute);
	private router = inject(Router);
	private auth = inject(AuthService);
	private sanitizer = inject(DomSanitizer);
	private cookie = inject(CookieService);

	private _sub: Subscription;

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
