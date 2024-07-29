import {ChangeDetectorRef, Component, OnInit, OnDestroy} from '@angular/core';
import {
	Router, Event as RouterEvent, ActivatedRoute,
	NavigationStart,
	NavigationEnd,
	NavigationCancel,
	NavigationError
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

import { SpaceListComponent } from './space/list/component';

@Component({
	selector: 'app-root',
	templateUrl: './app.component.html',
	styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit, OnDestroy {
	loading = true;

	mobileQuery: MediaQueryList;
	private _mobileQueryListener: () => void;

	scrollTop = 0;

	languages = [
		{code: 'en', label: 'English', icon: 'flag-icon flag-icon-gb'},
		{code: 'ru', label: 'Русский', icon: 'flag-icon flag-icon-ru'},
		// { code: 'es', label: 'Español', icon: 'flag-icon flag-icon-es'},
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

		let lang;
		const match = document.location.pathname.match(/\/(ru|en)\//);

		if (match === null) {
			const browserLang = translate.getBrowserLang();
			console.log('Browser Lang:' + browserLang);
			lang = browserLang.match(/ru|en/) ? browserLang : 'en';
		} else {
			console.log('url lang: ' + match[1]);
			lang = match[1];
		}

		const cookieLang = cookie.get('lang');
		if (cookieLang) {
			if (cookieLang !== lang) {
				console.log('redirect');
				console.log('Cookie Lang: ' + cookieLang);
				lang = cookieLang;
				this.change_language(lang);
			}
		}

		translate.use(lang);

		document.getElementsByTagName('html')[0].setAttribute('lang', lang);

		for (const item of this.languages) {
			if (item.code == lang) {
				this.current_lang_ = item;
			}
		}

		this.uiService.setCurLang(lang);
	}

	set_language() {
		this.cookie.set('lang', this.current_lang_.code, 365, '/');
		this.change_language();
	}

	change_language(toLang?): void {
		const match = document.location.pathname.match(/\/(ru|en)\//);
		if (match !== null) {
			const current = document.location.href;
			if (!toLang) {
				toLang = this.current_lang_.code;
			}
			const result = current.replace(match[0], ('\/' + toLang + '\/'));

			this.uiService.setCurLang(toLang);

			window.open(result, '_self');
		}
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
