import {ChangeDetectorRef, Component, OnInit, OnDestroy} from '@angular/core';
import {
	Router, Event as RouterEvent, ActivatedRoute,
	NavigationStart,
	NavigationEnd,
	NavigationCancel,
	NavigationError,
    RouterOutlet
} from '@angular/router';

import {MediaMatcher} from '@angular/cdk/layout';
import {Title} from '@angular/platform-browser';
import {TranslateService} from '@ngx-translate/core';
import {CookieService} from 'ngx-cookie-service';
import {Subscription} from 'rxjs';

import {AuthService} from './auth/service';
import {SpaceService} from './space/service';
import { UserBadgeService } from './user-badge/service';

import {UIService} from './ui.service';

import { CommonModule } from '@angular/common';
import { ReactiveFormsModule } from '@angular/forms';

// TODO: check is needed
import {DashboardComponent} from './schemes/dashboard/dashboard.component';
import {SchemeListComponent} from './schemes/list/list.component';
import {Create_Scheme_Dialog} from './schemes/list/create-scheme-dialog/create-scheme-dialog';
import {SchemeSearchComponent} from './schemes/search/search.component';
import {MessagesComponent} from './messages/messages.component';
import {LoginComponent} from './auth/login/login.component';
import {LogoutComponent} from './auth/logout/logout.component';
import {TgAuthComponent} from './tg-auth/tg-auth.component';
import {RememberPageLimitDirective} from './remember-page-limit.directive';
import {ConfirmationDialogComponent} from './confirmation-dialog/confirmation-dialog.component';
import { SpaceAddComponent } from './space/add/component';
import { SpaceDetailComponent } from './space/detail/component';
import { SpaceListComponent } from './space/list/component';
import { SpaceRequestSentComponent } from './space/request-sent/component';
import { ProgressSpinnerComponent } from './request-watcher/progress-spinner/component';
import { SpaceKeyValidatorDirective } from './space/add/space-key-validator';
import { SpaceInvitationListComponent } from './space/invitation-list/component';
import { SpaceLinkListComponent } from './space/link-list/component';
import { SpaceLinkJoinComponent } from './space/link-join/component';
import { SpaceAutoSelectComponent } from './space/auto-select/component';
import { UserBadgeComponent } from './user-badge/component';
import { ProjectListComponent } from './projects/list/list.component';
import { Create_Project_Dialog } from './projects/list/create-project-dialog/create-project-dialog';
import {SchemesDetailModule} from './schemes/schemes-detail.module';

import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {FormsModule} from '@angular/forms';
import { HttpClientModule, HttpClient, HTTP_INTERCEPTORS, provideHttpClient, withInterceptorsFromDi, withXsrfConfiguration } from '@angular/common/http';
import {MaterialModule} from './material.module';
import {UserSettingsModule} from './user-settings/user-settings.module';
import { ApiModule } from './api';
import {TranslateModule, TranslateLoader} from '@ngx-translate/core';

@Component({
	selector: 'app-root',
	standalone: true,
	imports: [
		CommonModule,
		RouterOutlet,


		ProgressSpinnerComponent,


		// TODO: check is needed
		ReactiveFormsModule,
//		DashboardComponent,
//		SchemeListComponent,
//		Create_Scheme_Dialog,
//		MessagesComponent,
//		SchemeSearchComponent,
//		LoginComponent,
//		LogoutComponent,
//		TgAuthComponent,
//		RememberPageLimitDirective,
//		ConfirmationDialogComponent,
//		SpaceAutoSelectComponent,
//		SpaceAddComponent,
//		SpaceInvitationListComponent,
//		SpaceLinkListComponent,
//		SpaceListComponent,
//		SpaceDetailComponent,
//		SpaceRequestSentComponent,
//		SpaceKeyValidatorDirective,
//		SpaceLinkJoinComponent,
//		UserBadgeComponent,
//		ProjectListComponent,
//		Create_Project_Dialog,
//
//		BrowserAnimationsModule,
//		//	  BrowserModule,
//		FormsModule,
//		ReactiveFormsModule,
//		SchemesDetailModule,
//		MaterialModule,
//		AngularMultiSelectModule,
//		UserSettingsModule,
//		ApiModule,
//		HttpClientModule,
//		TranslateModule,
	],
	templateUrl: './app.component.html',
	styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {
	loading = true;

	mobileQuery: MediaQueryList;
	private _mobileQueryListener: () => void;

	scrollTop = 0;

	languages = [
		{code: 'en', label: 'English', icon: 'fi fi-gb'},
		{code: 'ru', label: 'Русский', icon: 'fi fi-ru'},
		// { code: 'es', label: 'Español', icon: 'fi fi-es'},
	];

	current_lang_: any;
	cookieGot: boolean;
	showDropDown = false;
	authorized: boolean;
	initialized: boolean;
	private title$: Subscription;
	private _subAuth: Subscription;
	private _subSpace: Subscription;
	private _subSpaceEvent: Subscription;

	get isAdmin(): boolean {
		return this.auth.isAdmin();
	}

	constructor(
		public translate: TranslateService,
		private route: ActivatedRoute,
		private router: Router,
		public uiService: UIService,
		public cookie: CookieService,
		private changeDetectorRef: ChangeDetectorRef,
		media: MediaMatcher,
		private title: Title,
		private space: SpaceService,
		private auth: AuthService,
		private userBadges: UserBadgeService,
	) {
		this.cookieGot = this.cookie.get('cookie-agree') === 'true';

		this.router.events.subscribe((event: RouterEvent) => this.navigationInterceptor(event));

		this.mobileQuery = media.matchMedia('(max-width: 600px)');
		this._mobileQueryListener = () => changeDetectorRef.detectChanges();
		this.mobileQuery.addListener(this._mobileQueryListener);

		translate.addLangs(['en', 'ru']);
		// this language will be used as a fallback when a translation isn't found in the current language
		translate.setDefaultLang('en');
		// the lang to use, if the lang isn't available, it will use the current loader to get them
		// translate.use('ru');

		let lang = cookie.get('lang');
		if (!lang) {
			const browserLang = translate.getBrowserLang();
			console.log('Browser Lang:' + browserLang);

			lang = browserLang.match(/ru|en/) ? browserLang : 'en';
			this.cookie.set('lang', lang, 365, '/');
		}

		const fileLang = document.getElementsByTagName('html')[0].getAttribute('lang');
		if (fileLang != lang) {
			const url = new URL(window.location.href);
			url.searchParams.set('lang', lang);
			window.location.href = url.toString();
			return;
		}

		for (const item of this.languages) {
			if (item.code == lang) {
				this.current_lang_ = item;
				break;
			}
		}

		console.log("Lang:", lang, "current_lang_:", this.current_lang_);

		translate.use(lang);
		this.uiService.setCurLang(lang);
	}

	set_language() {
		this.cookie.set('lang', this.current_lang_.code, 365, '/');
		const url = new URL(window.location.href);
		url.searchParams.set('lang', this.current_lang_.code);
		window.location.href = url.toString();
	}

	ngOnInit() {
		this._subAuth = this.auth.isAuthorized().subscribe(ok => {
			this.authorized = ok;
			this.changeDetectorRef.detectChanges();
		});

		this._subSpace = this.space.isInitialized().subscribe(res => {
			this.initialized = true;

			const invitationSize = res.invitationSize;
			this.userBadges.spaceInvitationSize = invitationSize;
			this.userBadges.userNotificationSize += invitationSize;

			this.changeDetectorRef.detectChanges();
		});
	}

	ngOnDestroy(): void {
		this._subSpace.unsubscribe();
		this._subSpaceEvent.unsubscribe();
		this._subAuth.unsubscribe();
		this.title$.unsubscribe();
		this.mobileQuery.removeListener(this._mobileQueryListener);
	}

	navigationInterceptor(event: RouterEvent): void {
		if (event instanceof NavigationStart) {
			this.loading = true;
		}
		if (event instanceof NavigationEnd) {
			this.loading = false;
		}

		// Set loading state to false in both of the below events to hide the spinner in case a request fails
		if (event instanceof NavigationCancel) {
			this.loading = false;
		}
		if (event instanceof NavigationError) {
			this.loading = false;
		}
		this.changeDetectorRef.detectChanges();
	}

	onScroll($event) {
		this.scrollTop = $event.target.scrollTop;
	}

	cookieAgree() {
		this.cookie.set('cookie-agree', 'true', 365, '/');
		this.cookieGot = true;
	}

	toggleDropDown() {
		this.showDropDown = !this.showDropDown;
	}
}
