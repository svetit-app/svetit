import { enableProdMode, importProvidersFrom } from '@angular/core';
import { platformBrowserDynamic } from '@angular/platform-browser-dynamic';

import { createTranslateLoader } from './app/app.module';
import { environment } from './environments/environment';
import { AuthService } from './app/auth/service';
import { SpaceService } from './app/space/service';
import { SpaceVisitHolder } from './app/space/visit-holder';
import { RequestWatcherService } from './app/request-watcher/service';
import { SchemesService } from './app/schemes/schemes.service';
import { MessageService } from './app/message.service';
import { UIService } from './app/ui.service';
import { TranslateService, TranslateModule, TranslateLoader } from '@ngx-translate/core';
import { WebSocketBytesService } from './app/web-socket.service';
import { CookieService } from 'ngx-cookie-service';
import { FavService } from './app/fav.service';
import { ProjectService } from './app/projects/project.service';
import { HTTP_INTERCEPTORS, provideHttpClient, withInterceptorsFromDi, withXsrfConfiguration, HttpClient } from '@angular/common/http';
import { AuthInterceptor } from './app/auth/interceptor';
import { MatPaginatorIntl } from '@angular/material/paginator';
import { PaginatorIntlService } from './app/paginator-intl.service';
import { UserBadgeService } from './app/user-badge/service';
import { BASE_PATH, ApiModule } from './app/api';
import { provideAnimations } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { AppRoutingModule } from './app/app-routing.module';
import { SchemesDetailModule } from './app/schemes/schemes-detail.module';
import { TranslateHttpLoader } from '@ngx-translate/http-loader';
import { AngularMultiSelectModule } from 'angular2-multiselect-dropdown';
import { UserSettingsModule } from './app/user-settings/user-settings.module';
import { AppComponent } from './app/app.component';
import { bootstrapApplication } from '@angular/platform-browser';

if (environment.production) {
	enableProdMode();
}

bootstrapApplication(AppComponent, {
    providers: [
        importProvidersFrom(
        //	  BrowserModule,
        FormsModule, ReactiveFormsModule, AppRoutingModule, SchemesDetailModule, TranslateModule.forRoot({
            loader: {
                provide: TranslateLoader,
                useFactory: (createTranslateLoader),
                deps: [HttpClient]
            }
        }), AngularMultiSelectModule, UserSettingsModule, ApiModule),
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
        provideAnimations()
    ]
})
	.catch(err => console.log(err));
