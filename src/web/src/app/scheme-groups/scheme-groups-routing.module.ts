import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';



const routes: Routes = [
    {
        path: '',
        loadComponent: () => import('./scheme-groups/scheme-groups.component').then(m => m.SchemeGroupsComponent),
        children: [
            { path: 'group/:id', loadComponent: () => import('./edit-scheme-group/edit-scheme-group.component').then(m => m.EditSchemeGroupComponent), data: { title: '%GROUP%' } },
        ],
    },
];

@NgModule({
    declarations: [],
    imports: [
        RouterModule.forChild(routes),
    ],
    exports: [
        RouterModule
    ],
})
export class SchemeGroupsRoutingModule { }
