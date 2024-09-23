import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';




import {ChartFilterComponent} from './charts/chart-filter/chart-filter.component';

const reportRoutes: Routes = [
    {
        path: '',
        loadComponent: () => import('./reports.component').then(m => m.ReportsComponent),
        children: [
            {path: '', redirectTo: 'charts', pathMatch: 'full'},
            {
                path: 'charts',
                loadComponent: () => import('./charts/charts.component').then(m => m.ChartsComponent),
                data: { title: 'NAVIGATION.SCHEME.CHARTS' },
            },
            {
                path: 'export',
                loadComponent: () => import('./export/export.component').then(m => m.ExportComponent),
                data: { title: 'NAVIGATION.SCHEME.EXPORT' },
            },
        ]
    }
];

@NgModule({
    imports: [RouterModule.forChild(reportRoutes)],
    exports: [RouterModule]
})
export class ReportsRoutingModule {
}
