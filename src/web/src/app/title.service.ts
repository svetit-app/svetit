import { Injectable, inject } from '@angular/core';
import {Title} from '@angular/platform-browser';
import {filter, map} from 'rxjs/operators';
import {ActivatedRoute, NavigationEnd, Router} from '@angular/router';
import {Subscription} from 'rxjs';
import {TranslateService} from '@ngx-translate/core';

@Injectable({
	providedIn: 'root'
})
export class TitleService {
	private translate = inject(TranslateService);
	private title = inject(Title);
	private router = inject(Router);
	private route = inject(ActivatedRoute);

	private title$: Subscription;
	private variables: Record<string, string> = {};

	constructor() {
		this.title$ = this.router.events
			.pipe(
				filter((ev) => ev instanceof NavigationEnd),
				map(() => this.getTitle()),
			)
			.subscribe((title) => this.setTitle(title));
	}

	public setVariable(name: string, value: string): void {
		this.variables[name] = value;
		console.dir(this.variables);
		this.updateTitle();
	}

	private getTitle(): string[] {
		let child = this.route.firstChild;
		let title: string[] = [];
		while (child) {
			if (child.snapshot.data?.title) {
				let translated;
				if (child.snapshot.data?.title[0] === '%') {
					title.push(this.getVar(child.snapshot.data.title));
				} else {
					translated = this.translate.instant(child.snapshot.data?.title);
				}
				if (translated !== title[title.length - 1]) {
					title.push(translated);
				}
			}

			child = child.firstChild;
		}

		return title;
	}

	private getVar(name: string) {
		return this.variables[name];
	}

	private updateTitle() {
		const title = this.getTitle();
		this.setTitle(title);
	}

	private setTitle(title: string[]) {
		this.title.setTitle(
			title.filter(t => !!t).join(' - '),
		);
	}
}
