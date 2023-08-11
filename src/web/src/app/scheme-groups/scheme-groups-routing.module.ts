import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';
import {SchemeGroupsComponent} from './scheme-groups/scheme-groups.component';
import {EditSchemeGroupComponent} from './edit-scheme-group/edit-scheme-group.component';

const routes: Routes = [
    {
        path: '',
        component: SchemeGroupsComponent,
        children: [
            { path: 'group/:id', component: EditSchemeGroupComponent, data: { title: '%GROUP%' } },
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
