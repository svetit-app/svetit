import { Component, OnInit, inject } from '@angular/core';
import {ActivatedRoute} from '@angular/router';

import {AuthService} from '../service';

@Component({
    selector: 'app-logout',
    templateUrl: './logout.component.html',
    styleUrls: ['./logout.component.css'],
    standalone: true
})
export class LogoutComponent implements OnInit {
	private route = inject(ActivatedRoute);
	private auth = inject(AuthService);

	isErr = false;
	timeoutSecs = 5;

	ngOnInit() {
		let force = this.route.snapshot.queryParams['forceLogout'] === '1';
		force = force || !!this.route.snapshot.data?.complete;
		if (!force)
		{
			this.auth.GoToLogin(/*hideReferrer*/true);
			return;
		}

		this.isErr = this.route.snapshot.queryParams['isErr'] === '1';
		setTimeout(() => this.auth.GoToLogin(/*hideReferrer*/true), this.timeoutSecs * 1000);
	}
}
