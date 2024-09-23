import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';

import {authGuard} from '../auth/guard';
import {SchemeLoadGuard} from './scheme-load.guard';






import {ReportsModule} from './reports/reports.module';
import {PermissionGuard} from './permission.guard';





const schemeRoutes: Routes = [{
    path: '',
    children: [{
        path: ':name',
        data: { title: '%SCHEME%' },
        loadComponent: () => import('./scheme.component').then(m => m.SchemeComponent),
        canActivate: [SchemeLoadGuard],
        canActivateChild: [SchemeLoadGuard, PermissionGuard],
        children: [
            {path: '', redirectTo: 'detail', pathMatch: 'full'},
            {
                path: 'detail',
                loadComponent: () => import('../schemes/detail/detail.component').then(m => m.SchemeDetailComponent),
                data: {
                    req_perms: true,
                    title: 'NAVIGATION.SCHEME.DETAIL',
                },
            },
            {
                path: 'elements',
                loadComponent: () => import('./elements/elements.component').then(m => m.ElementsComponent),
                data: { req_perms: true },
                children: [
                    { path: 'sections', loadComponent: () => import('./elements/manage/manage.component').then(m => m.ManageComponent), data: { title: 'NAVIGATION.SCHEME.SECTIONS' } },
                    { path: 'devices', loadComponent: () => import('./elements/manage-devices/manage-devices.component').then(m => m.ManageDevicesComponent), data: { title: 'NAVIGATION.SCHEME.DEVICES' } },
                    { path: '', pathMatch: 'full', redirectTo: 'sections' },
                ],
            },
            {path: 'mnemoscheme', loadComponent: () => import('./mnemoscheme/mnemoscheme.component').then(m => m.MnemoschemeComponent), data: {req_perms: true, title: 'NAVIGATION.SCHEME.MNEMOSCHEME'}},
            {path: 'log', loadComponent: () => import('./log/log.component').then(m => m.LogComponent), data: {req_perms: true, title: 'NAVIGATION.SCHEME.LOG'}},
            {path: 'help', loadComponent: () => import('./doc/doc.component').then(m => m.DocComponent), data: {req_perms: true, title: 'NAVIGATION.SCHEME.HELP'}},
            // { path: 'group/:groupId/param', component: ParamComponent },
            {
                path: 'reports',
                loadChildren: () => import('./reports/reports.module').then(m => m.ReportsModule),
                canMatch: [authGuard],
                data: {req_perms: true}
            },
            {
                path: 'structure',
                loadChildren: () => import('./settings/settings.module').then(m => m.SettingsModule),
                data: {req_perms: true},
                canMatch: [authGuard]
            },
        ]
    }]
}];

@NgModule({
    imports: [ReportsModule, RouterModule.forChild(schemeRoutes)],
    exports: [RouterModule]
})
export class SchemeRoutingModule {
}
