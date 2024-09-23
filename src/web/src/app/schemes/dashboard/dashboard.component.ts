import { Component, OnDestroy, OnInit, inject } from '@angular/core';
import {Router, RouterModule} from '@angular/router';

import {SchemesService} from '../schemes.service';
import {FavService} from '../../fav.service';
import {TranslateService} from '@ngx-translate/core';
import { HttpClient } from '@angular/common/http';
import {SchemesList} from '../schemes-list';
import {Scheme} from '../../user';
import {combineLatest, concat} from 'rxjs';
import { SchemeStateComponent } from '../scheme-state/scheme-state.component';

@Component({
	selector: 'app-dashboard',
	templateUrl: './dashboard.component.html',
	styleUrls: ['./dashboard.component.css', '../../sections.css', '../schemes-list.css'],
	standalone: true,
	imports: [
		RouterModule,
		SchemeStateComponent,
	]
})
export class DashboardComponent extends SchemesList implements OnInit, OnDestroy {
	private router = inject(Router);
	private schemesService = inject(SchemesService);
	private favService = inject(FavService);

	favschemes: Scheme[];

	ngOnInit() {
		this.getSchemes();
	}

	getSchemes(): void {
		this.schemesService.getSchemes(5, 0, '-last_usage')
			.subscribe(data => {
				this.schemes = data.results.slice(0, 5);
				this.getStatuses();
				this.getFavSchemes();
			});
	}

	getFavSchemes(): void {
		this.favschemes = this.favService.getFavs() as Scheme[]; // preloading list from cookies

		const observables = this.favschemes.map(schemeInfo => this.schemesService.getScheme(schemeInfo.name));
		combineLatest(observables).subscribe(schemes => {
			this.favschemes = schemes;
			this.getStatuses(this.favschemes);
		});
	}
}
