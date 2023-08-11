import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';

import {authGuard} from '../auth/guard';
import {SchemeLoadGuard} from './scheme-load.guard';

import {SchemeDetailComponent} from '../schemes/detail/detail.component';

import {SchemeComponent} from './scheme.component';
import {ManageComponent} from './elements/manage/manage.component';
import {LogComponent} from './log/log.component';
import {ReportsModule} from './reports/reports.module';
import {PermissionGuard} from './permission.guard';
import {DocComponent} from './doc/doc.component';
import {ElementsComponent} from './elements/elements.component';
import {ManageDevicesComponent} from './elements/manage-devices/manage-devices.component';
import { MnemoschemeComponent } from './mnemoscheme/mnemoscheme.component';

const schemeRoutes: Routes = [{
    path: '',
    children: [{
        path: ':name',
        data: { title: '%SCHEME%' },
        component: SchemeComponent,
        canActivate: [SchemeLoadGuard],
        canActivateChild: [SchemeLoadGuard, PermissionGuard],
        children: [
            {path: '', redirectTo: 'detail', pathMatch: 'full'},
            {
                path: 'detail',
                component: SchemeDetailComponent,
                data: {
                    req_perms: true,
                    title: 'NAVIGATION.SCHEME.DETAIL',
                },
            },
            {
                path: 'elements',
                component: ElementsComponent,
                data: { req_perms: true },
                children: [
                    { path: 'sections', component: ManageComponent, data: { title: 'NAVIGATION.SCHEME.SECTIONS' } },
                    { path: 'devices', component: ManageDevicesComponent, data: { title: 'NAVIGATION.SCHEME.DEVICES' } },
                    { path: '', pathMatch: 'full', redirectTo: 'sections' },
                ],
            },
            {path: 'mnemoscheme', component: MnemoschemeComponent, data: {req_perms: true, title: 'NAVIGATION.SCHEME.MNEMOSCHEME'}},
            {path: 'log', component: LogComponent, data: {req_perms: true, title: 'NAVIGATION.SCHEME.LOG'}},
            {path: 'help', component: DocComponent, data: {req_perms: true, title: 'NAVIGATION.SCHEME.HELP'}},
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
