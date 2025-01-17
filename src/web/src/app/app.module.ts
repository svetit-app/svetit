//import { BrowserModule } from '@angular/platform-browser';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {NgModule} from '@angular/core';
import {FormsModule} from '@angular/forms';
import {ReactiveFormsModule} from '@angular/forms';
import { HttpClient, HTTP_INTERCEPTORS, provideHttpClient, withInterceptorsFromDi, withXsrfConfiguration } from '@angular/common/http';


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
import { SpaceVisitHolder } from './space/visit-holder';
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
import { SpaceAutoSelectComponent } from './space/auto-select/component';
import { UserBadgeComponent } from './user-badge/component';
import { UserBadgeService } from './user-badge/service';
import { ApiModule } from './api';
import { BASE_PATH } from './api';
import { ProjectListComponent } from './projects/list/list.component';
import { ProjectService } from './projects/project.service';
import { Create_Project_Dialog } from './projects/list/create-project-dialog/create-project-dialog';

