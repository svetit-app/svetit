import { AfterViewInit, ChangeDetectorRef, Component, OnDestroy, OnInit, ViewChild, ViewContainerRef, inject } from '@angular/core';
import { ActivatedRoute, NavigationEnd, Router, RouterLinkActive, RouterLink, RouterOutlet } from '@angular/router';
import {MediaMatcher} from '@angular/cdk/layout';
import { MatDialog, MatDialogRef, MatDialogTitle, MatDialogContent, MatDialogActions, MatDialogClose } from '@angular/material/dialog';
import {TranslateService} from '@ngx-translate/core';

import {Subscription, SubscriptionLike} from 'rxjs';

import {SchemeService} from './scheme.service';
import {Connection_State} from '../user';
import {ControlService, WebSockCmd} from './control.service';
import {AuthService} from '../auth/service';
import {FavService} from '../fav.service';
import {needSidebarHelper, NeedSidebar} from './sidebar.service';
import {Time_Info} from './scheme';
import {filter} from 'rxjs/operators';
import {TitleService} from '../title.service';
import { SchemeStateComponent } from '../schemes/scheme-state/scheme-state.component';

import { DragScrollComponent } from './drag-scroll.component';
import { MatTooltip } from '@angular/material/tooltip';
import { MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';
import { CdkScrollable } from '@angular/cdk/scrolling';

interface NavLink {
    link: string;
    query?: any;
    text: string;
    icon: string;
}

@Component({
    selector: 'app-scheme',
    templateUrl: './scheme.component.html',
    styleUrls: ['./scheme.component.css'],
    standalone: true,
    imports: [
    SchemeStateComponent,
    DragScrollComponent,
    MatTooltip,
    MatButton,
    MatIcon,
    RouterLinkActive,
    RouterLink,
    RouterOutlet
],
})
export class SchemeComponent implements OnInit, OnDestroy, AfterViewInit {
    translate = inject(TranslateService);
    schemeService = inject(SchemeService);
    private route = inject(ActivatedRoute);
    private controlService = inject(ControlService);
    private user = inject(AuthService);
    private dialog = inject(MatDialog);
    private router = inject(Router);
    private changeDetectorRef = inject(ChangeDetectorRef);
    private favService = inject(FavService);

    private readonly status_weight = {
        'Ok': 1,
        'Undefined': 2,
        'Warn': 3,
        'Error': 4,
    };

    @ViewChild('sidebar', {read: ViewContainerRef}) sidebarContainerRef: ViewContainerRef;
    @ViewChild('sidebarMobile', {read: ViewContainerRef}) sidebarMobileContainerRef: ViewContainerRef;

    haveSidebar = false;

    mobileQuery: MediaQueryList;
    private _mobileQueryListener: () => void;

    fillerNav: NavLink[] = [];

    status_checked = false;
    connection_str = ' '; // SchemeComponent.getConnectionString(false);

    connect_state: Connection_State = Connection_State.CS_DISCONNECTED;
    mod_state: boolean;
    loses_state: boolean;
    private active_route_component_: Component;

    get connected(): boolean {
        return this.connect_state !== Connection_State.CS_DISCONNECTED;
    }

    get messages(): any {
        const msgArray = [];
        for (const sect of this.schemeService.scheme.section) {
            for (const grp of sect.groups) {
                let status = 'Undefined',
                    status_text = '';

                if (grp.status_info !== undefined) {
                    status = grp.status_info.short_text;
                    status_text = grp.status_info.text;
                }

                if (status === 'Ok') {
                    continue;
                }

                msgArray.push({
                    section: sect.name,
                    section_id: sect.id,
                    group: grp.title ? grp.title : grp.type.title, // TODO: Why there are 'title' and 'type.title'?
                    group_id: grp.id,
                    status: this.status_weight[status],
                    text: status_text,
                });
            }
        }

        return msgArray;
    }

    private page_reload_dialog_ref: MatDialogRef<PageReloadDialogComponent> = undefined;

    dt_offset = 0;
    dt_tz_name = '';
    dt_interval: any;
    dt_text = '';

    private bytes_sub: SubscriptionLike;
    private opened_sub: SubscriptionLike;

    isFav = false;

    private getConnectionString(connected: boolean): string {
        return connected ? undefined : this.translate.instant('CONNECTION_PROBLEM');
    }

    constructor() {
        const changeDetectorRef = this.changeDetectorRef;
        const media = inject(MediaMatcher);

        this.mobileQuery = media.matchMedia('(max-width: 600px)');
        this._mobileQueryListener = () => changeDetectorRef.detectChanges();
        // this.mobileQuery.addListener(this._mobileQueryListener);
        this.mobileQuery.addEventListener('change', () => this.redrawSidebar_());
    }

    addMenu(name: string, icon: string): void {
        if (this.user.checkPermission('menu_' + name)) {
            this.fillerNav.push({link: name, text: this.translate.instant('NAVIGATION_TAB.' + name.toUpperCase()), icon: icon});
        }
    }

    ngOnInit() {
        this.addMenu('detail', 'perm_device_information');
        this.addMenu('mnemoscheme', 'image');
        this.addMenu('elements', 'build');
        this.addMenu('log', 'event_note');
        this.addMenu('structure', 'settings');
        this.addMenu('reports', 'show_chart');
        this.addMenu('help', 'help');

        this.getSchemeInfo();

        this.isFav = this.favService.isFav(this.schemeService.scheme.name);
    }

    ngAfterViewInit() {
        this.changeDetectorRef.detectChanges();
    }

    ngOnDestroy() {
        // this.mobileQuery.removeListener(this._mobileQueryListener);

        this.opened_sub.unsubscribe();
        this.bytes_sub.unsubscribe();

        this.controlService.close();
        this.schemeService.clear();
    }

    setConnectionState(state: Connection_State): void {
        this.connect_state = state;
        this.schemeService.isSchemeConnected =
            state !== Connection_State.CS_DISCONNECTED
            && state !== Connection_State.CS_DISCONNECTED_JUST_NOW
            && state !== Connection_State.CS_SERVER_DOWN;
    }

    clearTime(): void {
        if (this.dt_interval) {
            clearInterval(this.dt_interval);
            this.dt_text = '';
        }
    }

    getSchemeInfo(): void {
        this.schemeService.scheme.conn.subscribe(v => {
            const [connState, modState, losesState] = this.controlService.parseConnectNumber(v);
            this.setConnectionState(connState as Connection_State);
            this.mod_state = modState as boolean;
            this.loses_state = losesState as boolean;
        }).unsubscribe();

        this.bytes_sub = this.controlService.byte_msg.subscribe(msg => {

            if (msg.cmd === WebSockCmd.WS_CONNECTION_STATE) {
                if (msg.data === undefined) {
                    console.log('ConnectInfo without data');
                    return;
                }

                if (msg.scheme_id === 0) {
                    console.log('SCHEME_ID == 0');
                    this.setConnectionState(Connection_State.CS_SERVER_DOWN);
                    return;
                }

                const [connState, modState, losesState] = this.controlService.parseConnectState(msg.data);


                /* get connecton state */
                this.setConnectionState(connState);
                this.mod_state = modState;
                this.loses_state = losesState;

                if (!this.status_checked) {
                    this.status_checked = true;
                }
            } else if (msg.cmd === WebSockCmd.WS_STRUCT_MODIFY) {
                const view = new Uint8Array(msg.data);
                const structure_type = view[8];
                switch (structure_type) {
                    case 23: // STRUCT_TYPE_DEVICE_ITEM_VALUES
                    case 24: // STRUCT_TYPE_GROUP_MODE
                    case 25: // STRUCT_TYPE_GROUP_STATUS
                    case 26: // STRUCT_TYPE_GROUP_PARAM_VALUE
                        return;
                }

                this.connect_state = Connection_State.CS_CONNECTED_MODIFIED;

                if (!this.page_reload_dialog_ref) {
                    this.page_reload_dialog_ref = this.dialog.open(PageReloadDialogComponent, {width: '80%',});
                    this.page_reload_dialog_ref.afterClosed().subscribe(result => {
                        if (result) {
                            window.location.reload();
                        }
                        this.page_reload_dialog_ref = undefined;
                    });
                }
            } else if (msg.cmd === WebSockCmd.WS_TIME_INFO) {
                const info = this.controlService.parseTimeInfo(msg.data);
                this.startTimeTimer(info);
            }
        });

        this.opened_sub = this.controlService.opened.subscribe(opened => {
            this.connection_str = this.getConnectionString(opened);

            if (opened) {
                this.controlService.getConnectInfo();
            } else {
                this.clearTime();
                this.setConnectionState(Connection_State.CS_DISCONNECTED);
            }
        });

        this.controlService.open();
        this.schemeService.getTimeInfo().subscribe(info => this.startTimeTimer(info));
    }

    startTimeTimer(info: Time_Info) {
        if (info.utc_time) {
            const now = new Date();
            info.tz_offset += now.getTimezoneOffset() * 60;
            this.dt_offset = (now.getTime() - info.utc_time) + info.tz_offset * 1000;
            this.dt_tz_name = info.tz_name;
            if (!this.dt_interval) {
                this.gen_time_string();
                this.dt_interval = setInterval(() => this.gen_time_string(), 1000);
            }
        }
    }

    gen_time_string() {
        const dt = new Date();
        dt.setTime(dt.getTime() + this.dt_offset);

        const months = this.translate.instant('MONTHS');
        const t_num = (num: number): string => {
            return (num < 10 ? '0' : '') + num.toString();
        };

        this.dt_text = t_num(dt.getHours()) + ':' + t_num(dt.getMinutes()) + ':' + t_num(dt.getSeconds()) + ', ' +
            t_num(dt.getDate()) + ' ' + (months.length === 12 ? months[dt.getMonth()] : dt.getMonth()) + ' ' + dt.getFullYear();
    }

    closeMenu() {
        const el: HTMLInputElement = <HTMLInputElement>document.getElementById('localnav-menustate');
        el.checked = false;
    }

    fav() {
        this.favService.addToFav(this.schemeService.scheme.name, this.schemeService.scheme.title);
        this.isFav = this.favService.isFav(this.schemeService.scheme.name);
    }

    unfav() {
        this.favService.removeFromFav(this.schemeService.scheme.name);
        this.isFav = this.favService.isFav(this.schemeService.scheme.name);
    }

    onRouterOutletActivate(component: Component): void {
        this.active_route_component_ = component;
        this.redrawSidebar_();
    }

    private redrawSidebar_(): void {
        this.sidebarContainerRef.clear();
        this.sidebarMobileContainerRef.clear();

        const containerRef = this.mobileQuery.matches ? this.sidebarMobileContainerRef : this.sidebarContainerRef;

        this.haveSidebar = needSidebarHelper(this.active_route_component_);
        if (this.haveSidebar) {
            (this.active_route_component_ as NeedSidebar).getSidebarWidget(containerRef);
        }

        this.changeDetectorRef.detectChanges();
    }
}

@Component({
    templateUrl: './page-reload-dialog.component.html',
    standalone: true,
    imports: [
        MatDialogTitle,
        CdkScrollable,
        MatDialogContent,
        MatDialogActions,
        MatButton,
        MatDialogClose,
    ],
})
export class PageReloadDialogComponent {
    dialogRef = inject<MatDialogRef<PageReloadDialogComponent>>(MatDialogRef);
}
