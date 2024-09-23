import { Component, inject } from '@angular/core';
import { Router } from '@angular/router';

@Component({
    selector: 'app-space-request-sent',
    templateUrl: './component.html',
    styleUrls: ['./component.css'],
    standalone: true
})
export class SpaceRequestSentComponent {
	private router = inject(Router);

	spaceName: string;

	constructor() {
		const navigation = this.router.getCurrentNavigation();
		const state = navigation.extras.state as {spaceName: string};
		this.spaceName = state.spaceName;
	}
}
