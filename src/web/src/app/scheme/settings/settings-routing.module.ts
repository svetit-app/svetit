import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';

import {SettingsComponent} from './settings.component';
import {DevicesComponent} from './devices/devices.component';
import {SectionsComponent} from './sections/sections.component';
import {GroupTypesComponent} from './group-types/group-types.component';
import {DIG_Status_Category_Component} from './dig-status-category/dig-status-category.component';
import {SignTypesComponent} from './sign-types/sign-types.component';
import {Code_Item_Component} from './codes/codes.component';
import {PluginTypesComponent} from './plugin-types/plugin-types.component';
import {SaveTimersComponent} from './save-timers/save-timers.component';
import {DigModeTypeComponent} from './dig-mode-type/dig-mode-type.component';
import {TranslationComponent} from './translation/translation.component';
import {ValueViewComponent} from './value-view/value-view.component';
import {PendingChangesGuard} from './pending-changes.guard';

const settingsRoutes: Routes = [
    {
        path: '',
        data: { title: 'NAVIGATION.SCHEME.SETTINGS' },
        component: SettingsComponent,
        children: [
            {path: '', pathMatch: 'full'},
            {path: 'devices', component: DevicesComponent},
            {
                path: 'sections',
                children: [
                    {path: '', component: SectionsComponent, pathMatch: 'full', canDeactivate: [PendingChangesGuard]},
                    {
                        path: ':sctId',
                        children: [
                            {path: '', component: SectionsComponent, pathMatch: 'full'},
                            {path: 'group/:groupId', component: SectionsComponent},
                        ]
                    }
                ]
            },
            {path: 'grouptypes', component: GroupTypesComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'dig_status_category', component: DIG_Status_Category_Component, canDeactivate: [PendingChangesGuard]},
            {path: 'signtypes', component: SignTypesComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'plugin_type', component: PluginTypesComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'savetimers', component: SaveTimersComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'codes', component: Code_Item_Component, canDeactivate: [PendingChangesGuard]},
            {path: 'dig_mode_type', component: DigModeTypeComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'translation', component: TranslationComponent, canDeactivate: [PendingChangesGuard]},
            {path: 'value_view', component: ValueViewComponent, canDeactivate: [PendingChangesGuard]},
        ]
    }
];

@NgModule({
    imports: [
        RouterModule.forChild(settingsRoutes),
    ],
    exports: [
        RouterModule,
    ],
})
export class SettingsRoutingModule {
}
