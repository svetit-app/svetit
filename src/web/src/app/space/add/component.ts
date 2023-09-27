import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceAddComponent implements OnInit {

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
	}
}
