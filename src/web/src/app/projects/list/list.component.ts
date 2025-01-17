import { ChangeDetectorRef, Component, OnDestroy, OnInit, QueryList, ViewChild, ViewChildren, inject } from '@angular/core';
import { Router, RouterModule } from '@angular/router';
import { HttpClient } from '@angular/common/http';
import { MatPaginatorModule, PageEvent } from '@angular/material/paginator';
import { MatPaginator } from '@angular/material/paginator';
import { MatDialog } from '@angular/material/dialog';
import { TranslateService } from '@ngx-translate/core';

import { of, Subject, Subscription } from 'rxjs';
import { debounceTime, distinctUntilChanged, switchMap } from 'rxjs/operators';

import { Scheme} from '../../user';
import { ProjectService } from '../project.service';
import { AuthService } from '../../auth/service';
import { Create_Project_Dialog } from './create-project-dialog/create-project-dialog';
import { ProjectList } from '../project-list';
import { MatInputModule } from '@angular/material/input';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatButtonModule } from '@angular/material/button';
import { MatSelectModule } from '@angular/material/select';

@Component({
	selector: 'app-projects',
	templateUrl: './list.component.html',
	styleUrls: ['./list.component.css', '../project-list.css'],
	standalone: true,
	imports: [
		RouterModule, MatInputModule, MatFormFieldModule, MatButtonModule, MatSelectModule, MatPaginatorModule,
	]
})
export class ProjectListComponent extends ProjectList implements OnInit, OnDestroy {
	private router = inject(Router);
	private schemesService = inject(ProjectService);
	private auth = inject(AuthService);
	dialog = inject(MatDialog);
	private changeDetectorRef = inject(ChangeDetectorRef);

	timeout: any;
	start = 0;
	limit = 10;

	get isExtraList(): boolean {
		return this.auth.isExtraList();
	}

	searchString: Subject<string> = new Subject<string>();

	schemesSubs: Subscription;

	searchQ: Subject<string>;

	resultsLength = 0;

	@ViewChildren(MatPaginator) paginator: QueryList<MatPaginator>;
	citySelected = null;
	cities: any[];
	compSelected: any;
	comps: any[];
	pageEvent: PageEvent;

	@ViewChild('searchBox', {static: true}) searchBox;

	ngOnInit() {
		this.getSchemes();
	}

	search(value: string) {
		let v = value;

		if (this.citySelected) {
			v += '&city__id=' + this.citySelected;
		}

		if (this.compSelected) {
			v += '&company__id=' + this.compSelected;
		}

		this.getSchemes(v);
	}

	getSchemes(query: string = ''): void {
		this.ngOnDestroy();

		if (this.schemesSubs) {
			this.schemesSubs.unsubscribe();
		}

		if (this.statusItemSubs) {
			this.statusItemSubs.map(ss => ss.unsubscribe());
		}

		if (this.timeout) {
			clearTimeout(this.timeout);
		}

		if (this.paginator) {
			console.log(this.paginator);
		}

		this.schemesSubs = this.schemesService.getSchemes(this.limit, this.start, 'title',
			query)
			.subscribe(dat => {
				console.log(dat);
				this.resultsLength = dat.total;
				this.schemes = dat.list;

				this.schemesSubs.unsubscribe();
			});
	}

	detail(scheme: Scheme): void {
		this.router.navigate([`/detail/${scheme.id}/`]);
	}

	getPaginatorData(event: PageEvent) {
		console.log(event);

		const q = this.searchBox.nativeElement.value;

		this.limit = event.pageSize;
		this.start = event.pageIndex;

		console.log(q);
		this.search(q);

		this.changeDetectorRef.detectChanges();
		return event;
	}

	open_create_scheme_dialog(): void
	{
		const dialogRef = this.dialog.open(Create_Project_Dialog, { width: '80%', data: { cities: this.cities, comps: this.comps } });

		dialogRef.afterClosed().subscribe(new_scheme => {
			if (!new_scheme)
				return;

			this.schemesService.create_scheme(new_scheme)
				.subscribe(scheme => {
					//if (scheme)
						//this.schemes.push(scheme);
				});
		});
	}
}
