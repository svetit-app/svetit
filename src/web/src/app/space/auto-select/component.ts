import { Component, OnInit } from '@angular/core';

@Component({
	selector: 'app-space-auto-select',
	templateUrl: './space-auto-select.component.html',
	styleUrls: ['./space-auto-select.component.css']
})
export class SpaceAutoSelectComponent implements OnInit {
	ngOnInit() {
		const spaceKey = localStorage.getItem("spaceKey")
		if (spaceKey) {
			// TODO: check space key
		}
	}
}
