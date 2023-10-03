import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { DashboardComponent } from './schemes/dashboard/dashboard.component';
import { SchemeListComponent } from './schemes/list/list.component';
import { SchemeDetailComponent } from './schemes/detail/detail.component';
import { authGuard } from './auth/guard';
import { spaceGuard } from './space/guard';
import { LogoutComponent } from "./auth/logout/logout.component";
import { LoginComponent } from "./auth/login/login.component";
import { UserTestComponent } from "./user/user/user.component";
import { SpaceListComponent } from "./space/list/component";
import { SpaceAddComponent } from "./space/add/component";
import { SpaceDetailComponent } from "./space/detail/component";
import { SpaceRequestSentComponent } from './space/space-request-sent/component';
import { TgAuthComponent } from './tg-auth/tg-auth.component';

const routes: Routes = [
	{path: 'login', component: LoginComponent, data: {title: 'NAVIGATION.LOGIN'}},
	{path: 'login/complete', component: LoginComponent, data: {title: 'NAVIGATION.LOGIN', complete: true}},
	{path: 'logout', component: LogoutComponent, data: {title: 'NAVIGATION.LOGOUT'}},
	{path: 'logout/complete', component: LogoutComponent, data: {title: 'NAVIGATION.LOGOUT', complete: true}},
	{path: '', canActivateChild: [authGuard, spaceGuard], children: [
		{path: '', redirectTo: '/dashboard', pathMatch: 'full'},
		{path: 'usertest', component: UserTestComponent, data: {title: 'NAVIGATION.LOGOUT'}},
		{path: 'space/list', component: SpaceListComponent, data: {title: 'NAVIGATION.SPACELIST'}},
		{path: 'space/add', component: SpaceAddComponent, data: {title: 'NAVIGATION.SPACEADD'}},
		{path: 'space/:id', component: SpaceDetailComponent, data: {title: 'NAVIGATION.SPACEDETAIL'}},
		{path: 'space/add/request/:id', component: SpaceRequestSentComponent, data: {title: 'NAVIGATION.SPACEREQUESTSENT'}},
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
