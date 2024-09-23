import {NgModule} from '@angular/core';
import {RouterModule, Routes} from '@angular/router';
import {NotificationsComponent} from './notifications/notifications.component';




const routes: Routes = [
  {
    path: '',
    loadComponent: () => import('./user-settings.component').then(m => m.UserSettingsComponent),
    children: [
      {
          path: 'details',
          loadComponent: () => import('./user-details/user-details.component').then(m => m.UserDetailsComponent),
      },
      {
          path: 'scheme-groups',
          loadComponent: () => import('./scheme-groups/scheme-groups.component').then(m => m.SchemeGroupsComponent),
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
