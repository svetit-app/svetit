//import { BrowserModule } from '@angular/platform-browser';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {NgModule} from '@angular/core';
import {FormsModule} from '@angular/forms';
import {ReactiveFormsModule} from '@angular/forms';
import {HttpClient, HttpClientModule, HttpClientXsrfModule, HTTP_INTERCEPTORS} from '@angular/common/http';

import {MaterialModule} from './material.module';
import {AngularMultiSelectModule} from 'angular2-multiselect-dropdown';

import {SchemesDetailModule} from './schemes/schemes-detail.module';
import {AppComponent} from './app.component';

import {DashboardComponent} from './schemes/dashboard/dashboard.component';
import {SchemeListComponent} from './schemes/list/list.component';
import {Create_Scheme_Dialog} from './schemes/list/create-scheme-dialog/create-scheme-dialog';
import {SchemeSearchComponent} from './schemes/search/search.component';
import {SchemesService} from './schemes/schemes.service';
import {WebSocketBytesService} from './web-socket.service';
import {MessagesComponent} from './messages/messages.component';
import {MessageService} from './message.service';
import {AppRoutingModule} from './app-routing.module';
import {LoginComponent} from './auth/login/login.component';
import {LogoutComponent} from './auth/logout/logout.component';
import {AuthService} from './auth/service';
import {AuthInterceptor} from './auth/interceptor';
import {TranslateModule, TranslateLoader, TranslateService} from '@ngx-translate/core';
import {TranslateHttpLoader} from '@ngx-translate/http-loader';
import {UIService} from './ui.service';
import {UserSettingsModule} from './user-settings/user-settings.module';
import {CookieService} from 'ngx-cookie-service';
import {FavService} from './fav.service';
import {TgAuthComponent} from './tg-auth/tg-auth.component';
import {RememberPageLimitDirective} from './remember-page-limit.directive';
import {PaginatorIntlService} from './paginator-intl.service';
import {MatPaginatorIntl} from '@angular/material/paginator';
import {ConfirmationDialogComponent} from './confirmation-dialog/confirmation-dialog.component';
import {Title} from '@angular/platform-browser';
import { SpaceService } from './space/service';
import { SpaceAddComponent } from './space/add/component';
import { SpaceDetailComponent } from './space/detail/component';
import { SpaceListComponent } from './space/list/component';
import { SpaceRequestSentComponent } from './space/request-sent/component';
import { ProgressSpinnerComponent } from './request-watcher/progress-spinner/component';
import { RequestWatcherService } from './request-watcher/service';
import { SpaceKeyValidatorDirective } from './space/add/space-key-validator';
import { SpaceInvitationListComponent } from './space/invitation-list/component';
import { SpaceLinkListComponent } from './space/link-list/component';
import { SpaceLinkJoinComponent } from './space/link-join/component';
import { UserBadgeComponent } from './user-badge/component';
import { UserBadgeService } from './user-badge/service';
import { ApiModule } from './api';
import { BASE_PATH } from './api';
import { ProjectListComponent } from './projects/list/list.component';
import { ProjectService } from './projects/project.service';
import { Create_Project_Dialog } from './projects/list/create-project-dialog/create-project-dialog';

export function createTranslateLoader(http: HttpClient) {
	return new TranslateHttpLoader(http, './assets/i18n/', '.json');
}

@NgModule({
	declarations: [
		AppComponent,
		DashboardComponent,
		SchemeListComponent,
		Create_Scheme_Dialog,
		MessagesComponent,
		SchemeSearchComponent,
		LoginComponent,
		LogoutComponent,
		TgAuthComponent,
		RememberPageLimitDirective,
		ConfirmationDialogComponent,
		SpaceAddComponent,
		SpaceInvitationListComponent,
		SpaceLinkListComponent,
		SpaceListComponent,
		SpaceDetailComponent,
		SpaceRequestSentComponent,
		ProgressSpinnerComponent,
		SpaceKeyValidatorDirective,
		SpaceLinkJoinComponent,
		UserBadgeComponent
		ProjectListComponent,
		Create_Project_Dialog,
	],
	imports: [
		BrowserAnimationsModule,
		//	  BrowserModule,
		FormsModule,
		ReactiveFormsModule,
		AppRoutingModule,
		SchemesDetailModule,
		HttpClientModule,
		HttpClientXsrfModule.withOptions({
			cookieName: 'csrftoken',
			headerName: 'X-CSRFToken',
		}),
		TranslateModule.forRoot({
			loader: {
				provide: TranslateLoader,
				useFactory: (createTranslateLoader),
				deps: [HttpClient]
			}
		}),
		MaterialModule,
		AngularMultiSelectModule,
		UserSettingsModule,
		ApiModule,
	],
	providers: [
		AuthService,
		SpaceService,
		RequestWatcherService,
		SchemesService,
		MessageService,
		UIService,
		TranslateService,
		WebSocketBytesService,
		CookieService,
		FavService,
		ProjectService,
		{
			provide: HTTP_INTERCEPTORS,
			useClass: AuthInterceptor,
			multi: true
		},
		{
			provide: MatPaginatorIntl,
			useFactory: (translate) => {
				const service = new PaginatorIntlService();
				service.injectTranslateService(translate);
				return service;
			},
			deps: [TranslateService]
		},
		UserBadgeService,
		{ provide: BASE_PATH, useValue: '/api' },
	],
	bootstrap: [AppComponent]
})
export class AppModule {
}
