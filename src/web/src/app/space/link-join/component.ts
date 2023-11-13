import { Component } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { SpaceService } from '../service';
import { Space } from '../model';

@Component({
	selector: 'app-space-link-join',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceLinkJoinComponent {
	token: string;
	targetSpace: Space = null;

	constructor(
		private route: ActivatedRoute,
		private space: SpaceService,
	) {}

	ngOnInit() {
		this.token = this.route.snapshot.paramMap.get('token');
		if (this.token) {
			this.initPage(this.token);
		}
	}

	initPage(token: string) {
		this.space.getLinkByToken(token)
			.subscribe(res => {
				if (res){
					this.space.getById(res.spaceId)
						.subscribe(res => {
							if (res) {
								this.targetSpace = res;
							}
						});
				}
			});
	}

	sendJoinRequest() {
		this.space.joinByLink(this.token)
			.subscribe(res => {
				if (res) {
					alert("Вы успешно присоединились к пространству.");
				} else {
					alert("Ошибка присоединения к пространству.");
				}
			});
	}
}
