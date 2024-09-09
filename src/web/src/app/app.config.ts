import { ApplicationConfig, provideZoneChangeDetection, importProvidersFrom } from '@angular/core';
import { provideRouter, withDebugTracing } from '@angular/router';

import { routes } from './app.routes';

// TODO: check is needed
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
import { BASE_PATH } from './api';
import { ProjectService } from './projects/project.service';
import { HttpClient, HTTP_INTERCEPTORS, provideHttpClient, withInterceptorsFromDi, withXsrfConfiguration } from '@angular/common/http';
import {TranslateModule, TranslateLoader, TranslateService} from '@ngx-translate/core';

import {TranslateHttpLoader} from '@ngx-translate/http-loader';
export function createTranslateLoader(http: HttpClient) {
	return new TranslateHttpLoader(http, './assets/i18n/', '.json');
}

export const appConfig: ApplicationConfig = {
	providers: [
		provideZoneChangeDetection({ eventCoalescing: true }),
		provideRouter(routes,
			// withDebugTracing(),
		),

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
		provideHttpClient(withInterceptorsFromDi(), withXsrfConfiguration({
		    cookieName: 'csrftoken',
		    headerName: 'X-CSRFToken',
		})),

		importProvidersFrom(
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
