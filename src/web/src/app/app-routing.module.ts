import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';




import { authGuard } from './auth/guard';
import { spaceInfoGuard } from './space/guard';






import { SpaceAutoSelectComponent } from './space/auto-select/component';




const routes: Routes = [
	{path: 'auth', children: [
		{path: 'login', loadComponent: () => import('./auth/login/login.component').then(m => m.LoginComponent), data: {title: 'NAVIGATION.LOGIN'}},
		{path: 'login/complete', loadComponent: () => import('./auth/login/login.component').then(m => m.LoginComponent), data: {title: 'NAVIGATION.LOGIN', complete: true}},
		{path: 'logout', loadComponent: () => import('./auth/logout/logout.component').then(m => m.LogoutComponent), data: {title: 'NAVIGATION.LOGOUT'}},
		{path: 'logout/complete', loadComponent: () => import('./auth/logout/logout.component').then(m => m.LogoutComponent), data: {title: 'NAVIGATION.LOGOUT', complete: true}},
	]},
	{path: '', canActivateChild: [authGuard, spaceInfoGuard], children: [
		{path: '', redirectTo: '/space/auto-select', pathMatch: 'full'},
		{path: 'space/auto-select', component: SpaceAutoSelectComponent, data: {title: 'NAVIGATION.SPACEAUTOSELECT'}},
		{path: 'space/list', loadComponent: () => import('./space/list/component').then(m => m.SpaceListComponent), data: {title: 'NAVIGATION.SPACELIST'}},
		{path: 'space/add', loadComponent: () => import('./space/add/component').then(m => m.SpaceAddComponent), data: {title: 'NAVIGATION.SPACEADD'}},
		{path: 'space/detail/:key', loadComponent: () => import('./space/detail/component').then(m => m.SpaceDetailComponent), data: {title: 'NAVIGATION.SPACEDETAIL'}},
		{path: 'space/add/request', loadComponent: () => import('./space/request-sent/component').then(m => m.SpaceRequestSentComponent), data: {title: 'NAVIGATION.SPACEREQUESTSENT'}},
		{path: 'space/link/:token', loadComponent: () => import('./space/link-join/component').then(m => m.SpaceLinkJoinComponent), data: {title: 'NAVIGATION.SPACELINKJOIN'}},
		{path: 'dashboard', loadComponent: () => import('./schemes/dashboard/dashboard.component').then(m => m.DashboardComponent), data: {title: 'NAVIGATION.DASHBOARD'}},
		{path: 'list', loadComponent: () => import('./schemes/list/list.component').then(m => m.SchemeListComponent), data: {title: 'NAVIGATION.LIST'}},
		{path: 'projects', loadComponent: () => import('./projects/list/list.component').then(m => m.ProjectListComponent), data: {title: 'NAVIGATION.PROJECTS'}},
		{path: 'detail/:name', loadComponent: () => import('./schemes/detail/detail.component').then(m => m.SchemeDetailComponent), data: {title: 'NAVIGATION.DETAIL'}},
		{
			path: 'scheme-groups',
			data: {title: 'NAVIGATION.GROUP_MANAGEMENT'},
			loadChildren: () => import('./scheme-groups/scheme-groups.module').then(m => m.SchemeGroupsModule),
			canMatch: [authGuard],
		},
		{
			path: 'scheme',
			loadChildren: () => import('./scheme/scheme.module').then(m => m.SchemeModule),
			canMatch: [authGuard]
		},
		{
			path: 'user',
			loadChildren: () => import('./user-settings/user-settings.module').then(m => m.UserSettingsModule),
			canMatch: [authGuard],
		},

		{path: 'tg_auth/:token', loadComponent: () => import('./tg-auth/tg-auth.component').then(m => m.TgAuthComponent)},
	]},
	// otherwise redirect to home
	{ path: '**', redirectTo: '/space/auto-select' }
];

@NgModule({
	imports: [ RouterModule.forRoot(routes, { /*enableTracing: true*/ }) ],
	exports: [ RouterModule ]
})
export class AppRoutingModule {}
