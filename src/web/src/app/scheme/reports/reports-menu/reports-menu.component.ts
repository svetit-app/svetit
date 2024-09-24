import { Component, OnInit, inject } from '@angular/core';
import { ActivatedRoute, NavigationEnd, Router, RouterLink, RouterLinkActive } from '@angular/router';
import {filter} from 'rxjs/operators';

import { ChartFilterComponent } from '../charts/chart-filter/chart-filter.component';

@Component({
    selector: 'app-reports-menu',
    templateUrl: './reports-menu.component.html',
    styleUrls: ['./reports-menu.component.css'],
    standalone: true,
    imports: [RouterLink, RouterLinkActive, ChartFilterComponent]
})
export class ReportsMenuComponent implements OnInit {
    private router = inject(Router);

    showChartFilter: boolean;

    constructor() {
        this.router.events
            .pipe(filter(ev => ev instanceof NavigationEnd))
            .subscribe((ev: NavigationEnd) => this.toggleChartFilter(ev.urlAfterRedirects));

        this.toggleChartFilter(this.router.url);
    }

    private toggleChartFilter(url: string) {
        const urlSegments = url.split('/');
        const lastSegment = urlSegments[urlSegments.length - 1];

        this.showChartFilter = lastSegment === 'charts';
    }

    ngOnInit(): void {
    }
}
