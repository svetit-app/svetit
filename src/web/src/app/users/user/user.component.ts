import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

@Component({
	selector: 'app-user-test',
	templateUrl: './user.component.html',
	styleUrls: ['./user.component.css']
})
export class UserTestComponent implements OnInit {

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}
