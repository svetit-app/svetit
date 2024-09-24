import { ApplicationConfig, provideZoneChangeDetection, importProvidersFrom, CSP_NONCE } from '@angular/core';
import { provideRouter, withDebugTracing } from '@angular/router';
import { provideAnimations } from '@angular/platform-browser/animations';

import { routes } from './app.routes';

// TODO: check is needed
import { SchemesDetailModule } from './schemes/schemes-detail.module';
import { AngularMultiSelectModule } from 'angular2-multiselect-dropdown';
import { UserSettingsModule } from './user-settings/user-settings.module';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import {SchemesService} from './schemes/schemes.service';
import {WebSocketBytesService} from './web-socket.service';
import {MessageService} from './message.service';
import {AuthService} from './auth/service';
import {AuthInterceptor} from './auth/interceptor';
import {UIService} from './ui.service';
import {CookieService} from 'ngx-cookie-service';
import {FavService} from './fav.service';
import {PaginatorIntlService} from './paginator-intl.service';
import {MatPaginatorIntl} from '@angular/material/paginator';
import {Title} from '@angular/platform-browser';
import { SpaceService } from './space/service';
import { SpaceVisitHolder } from './space/visit-holder';
import { RequestWatcherService } from './request-watcher/service';
import { UserBadgeService } from './user-badge/service';
import { BASE_PATH, ApiModule } from './api';
import { ProjectService } from './projects/project.service';
import { HttpClient, HTTP_INTERCEPTORS, provideHttpClient, withInterceptorsFromDi, withXsrfConfiguration } from '@angular/common/http';
import {TranslateModule, TranslateLoader, TranslateService} from '@ngx-translate/core';

import {TranslateHttpLoader} from '@ngx-translate/http-loader';
export function createTranslateLoader(http: HttpClient) {
	return new TranslateHttpLoader(http, './assets/i18n/', '.json');
}

const nonce = (
	document.querySelector('meta[name="CSP_NONCE"]') as HTMLMetaElement
)?.content;

export const appConfig: ApplicationConfig = {
	providers: [
		{
			provide: CSP_NONCE,
			useValue: nonce,
		},
		provideZoneChangeDetection({ eventCoalescing: true }),
		provideRouter(routes,
			//withDebugTracing(),
		),
		provideAnimations(),

		// TODO: check is needed
		AuthService,
		SpaceService,
		SpaceVisitHolder,
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
		provideHttpClient(withInterceptorsFromDi()),

		importProvidersFrom(
			FormsModule, ReactiveFormsModule, SchemesDetailModule, AngularMultiSelectModule, UserSettingsModule, ApiModule,
			TranslateModule.forRoot({
				loader: {
					provide: TranslateLoader,
					useFactory: (createTranslateLoader),
					deps: [HttpClient]
				}
			}),
		),
	]
};
