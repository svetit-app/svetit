import { Component, inject } from '@angular/core';
import { ActivatedRoute, NavigationExtras, Router } from '@angular/router';
import { SpaceService } from '../service';
import { Space } from '../../api';

@Component({
	selector: 'app-space-link-join',
	templateUrl: './component.html',
	styleUrls: ['./component.css'],
	standalone: true
})
export class SpaceLinkJoinComponent {
	private route = inject(ActivatedRoute);
	private space = inject(SpaceService);
	private router = inject(Router);

	linkId: string;
	targetSpace: Space;

	ngOnInit() {
		this.linkId = this.route.snapshot.paramMap.get('token');
		this.space.getByLink(this.linkId)
			.subscribe(res => this.targetSpace = res);
	}

	sendJoinRequest() {
		this.space.joinByLink(this.linkId)
			.subscribe(res => {
				const navigationExtras: NavigationExtras = {state: {spaceName: this.targetSpace.name}};
				this.router.navigate(['space/add/request'], navigationExtras);
			});
	}
}
