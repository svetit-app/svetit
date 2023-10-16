import { Component } from '@angular/core';
import { Router } from '@angular/router';

@Component({
	selector: 'app-space-request-sent',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceRequestSentComponent {

	spaceName: string;

	constructor(private router: Router) {
		const navigation = this.router.getCurrentNavigation();
		const state = navigation.extras.state as {spaceName: string};
		this.spaceName = state.spaceName;
	}
	
}
