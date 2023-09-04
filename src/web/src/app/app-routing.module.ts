import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { DashboardComponent } from './schemes/dashboard/dashboard.component';
import { SchemeListComponent } from './schemes/list/list.component';
import { SchemeDetailComponent } from './schemes/detail/detail.component';
import { authGuard } from './auth/guard';
import { workspaceGuard } from './workspace/guard';
import { LogoutComponent } from "./auth/logout/logout.component";
import { LoginComponent } from "./auth/login/login.component";
import { UserTestComponent } from "./user/user/user.component";
import { HelloComponent } from "./workspace/hello/component";
import { TgAuthComponent } from './tg-auth/tg-auth.component';

const routes: Routes = [
	{path: 'login', component: LoginComponent, data: {title: 'NAVIGATION.LOGIN'}},
	{path: 'logout', component: LogoutComponent, data: {title: 'NAVIGATION.LOGOUT'}},
	{path: 'hello', component: HelloComponent, data: {title: 'NAVIGATION.HELLO'}, canActivate: [authGuard]},
	{path: '', canActivateChild: [authGuard, workspaceGuard], children: [
		{path: '', redirectTo: '/dashboard', pathMatch: 'full'},
		{path: 'usertest', component: UserTestComponent, data: {title: 'NAVIGATION.LOGOUT'}},

		{ path: 'dashboard', component: DashboardComponent, data: {title: 'NAVIGATION.DASHBOARD'}},
		{ path: 'list', component: SchemeListComponent, data: {title: 'NAVIGATION.LIST'}},
		{ path: 'detail/:name', component: SchemeDetailComponent, data: {title: 'NAVIGATION.DETAIL'}},
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
		
		{path: 'tg_auth/:token', component: TgAuthComponent},
	]},
	// otherwise redirect to home
	{ path: '**', redirectTo: '/dashboard' }
];

@NgModule({
	imports: [ RouterModule.forRoot(routes, { /*enableTracing: true*/ }) ],
	exports: [ RouterModule ]
})
export class AppRoutingModule {}
