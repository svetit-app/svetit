import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';
import {NotificationsComponent} from './notifications/notifications.component';
import {UserSettingsComponent} from './user-settings.component';
import {UserDetailsComponent} from './user-details/user-details.component';
import {SchemeGroupsComponent} from './scheme-groups/scheme-groups.component';

const routes: Routes = [
  {
    path: '',
    component: UserSettingsComponent,
    children: [
      {
          path: 'details',
          component: UserDetailsComponent,
      },
      {
          path: 'scheme-groups',
          component: SchemeGroupsComponent,
      },
      {
          path: '',
          pathMatch: 'full',
          redirectTo: 'details',
      },
    ]
  }
];

@NgModule({
  imports: [ RouterModule.forChild(routes) ],
  exports: [ RouterModule ]
})
export class UserSettingsRoutingModule { }
