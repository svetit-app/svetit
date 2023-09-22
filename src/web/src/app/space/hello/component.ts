import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

@Component({
	selector: 'app-hello',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class HelloComponent implements OnInit {

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}
