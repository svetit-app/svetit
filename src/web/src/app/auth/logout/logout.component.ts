import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

import {AuthService} from '../service';

@Component({
	selector: 'app-logout',
	templateUrl: './logout.component.html',
	styleUrls: ['./logout.component.css']
})
export class LogoutComponent implements OnInit {
	isErr = false;
	timeoutSecs = 5;

	constructor(
		private route: ActivatedRoute,
		private auth: AuthService,
	) {
	}

	ngOnInit() {
		this.isErr = this.route.snapshot.queryParams['isErr'] === '1';
		setTimeout(() => this.auth.goToLogin(), this.timeoutSecs * 1000);
	}
}
