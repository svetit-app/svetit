import {NgModule} from '@angular/core';
import {CommonModule} from '@angular/common';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';
import {HttpClient} from '@angular/common/http';
import {TranslateModule, TranslateLoader} from '@ngx-translate/core';
import {TranslateHttpLoader} from '@ngx-translate/http-loader';

import {MaterialModule} from '../material.module';
import {SchemesDetailModule} from '../schemes/schemes-detail.module';

import {SchemeRoutingModule} from './scheme-routing.module';

import {NoSanitizePipe} from './no-sanitize.pipe';
import {SchemeLoadGuard} from './scheme-load.guard';
import {ControlService} from './control.service';
import {SchemeService} from './scheme.service';
import {SidebarService} from './sidebar.service';
import {SettingsService} from './settings.service';

import {SchemeComponent, PageReloadDialogComponent} from './scheme.component';
import {ManageComponent} from './elements/manage/manage.component';
import {LogComponent} from './log/log.component';
import {ParamComponent} from './param/param.component';
import {GroupStatusComponent} from './group-status/group-status.component';
import {DevItemValueComponent, HoldingRegisterDialogComponent} from './dev-item-value/dev-item-value.component';
import {VideoStreamDialogComponent} from './dev-item-value/video-stream-dialog/video-stream-dialog.component';
import {DragScrollComponent} from './drag-scroll.component';
import {ParamItemComponent} from './param-item/param-item.component';
import {SchemeSectionComponent} from './scheme-section/scheme-section.component';
import {DocComponent} from './doc/doc.component';
import {StatusManageDialogComponent} from './status-manage-dialog/status-manage-dialog.component';
import {StatusManageItemComponent} from './status-manage-dialog/status-manage-item/status-manage-item.component';
import {HelpItemComponent} from './doc/help-item/help-item.component';
import {SectionDetailDialogComponent} from './elements/manage/section-detail-dialog/section-detail-dialog.component';
import {DeviceItemGroupDetailDialogComponent} from './elements/manage/device-item-group-detail-dialog/device-item-group-detail-dialog.component';
import {DeviceItemDetailDialogComponent} from './elements/manage/device-item-detail-dialog/device-item-detail-dialog.component';
import {ElementsComponent} from './elements/elements.component';
import {ManageDevicesComponent} from './elements/manage-devices/manage-devices.component';
import {ParamTypeFormComponent} from './elements/manage/param-type-form/param-type-form.component';
import {DeviceDetailDialogComponent} from './elements/manage/device-detail-dialog/device-detail-dialog.component';
import {DeviceItemTypeDetailDialogComponent} from './elements/manage/device-item-type-detail-dialog/device-item-type-detail-dialog.component';
import {DeviceItemGroupTypeDetailDialogComponent} from './elements/manage/device-item-group-type-detail-dialog/device-item-group-type-detail-dialog.component';
import {SignTypeDetailDialogComponent} from './elements/manage/sign-type-detail-dialog/sign-type-detail-dialog.component';
import { PluginDetailDialogComponent } from './elements/manage/plugin-detail-dialog/plugin-detail-dialog.component';
import { ElementsMenuComponent } from './elements/elements-menu/elements-menu.component';
import { MnemoschemeComponent } from './mnemoscheme/mnemoscheme.component';
import { DeviceItemGroupComponent } from './device-item-group/device-item-group.component';
import {ParamsDialogComponent} from './device-item-group/params-dialog/params-dialog.component';
import {Title} from '@angular/platform-browser';
import { LogSidebarComponent } from './log/log-sidebar/log-sidebar.component';
import {AngularMultiSelectModule} from 'angular2-multiselect-dropdown';
import {MatProgressBarModule} from '@angular/material/progress-bar';

export function HttpLoaderFactory(httpClient: HttpClient) {
    return new TranslateHttpLoader(httpClient, './assets/i18n/', '.json');
}

@NgModule({
    imports: [
        CommonModule,
        FormsModule,
        ReactiveFormsModule,
        SchemeRoutingModule,
        MaterialModule,
        SchemesDetailModule,
        TranslateModule.forChild({
            loader: {
                provide: TranslateLoader,
                useFactory: HttpLoaderFactory,
                deps: [HttpClient]
            }
        }),
        AngularMultiSelectModule,
        MatProgressBarModule,
    ],
    declarations: [
        SchemeComponent,
        PageReloadDialogComponent,
        ManageComponent,
        StatusManageDialogComponent,
        HoldingRegisterDialogComponent,
        VideoStreamDialogComponent,
        LogComponent,
        ParamComponent,
        GroupStatusComponent,
        DevItemValueComponent,
        DragScrollComponent,
        DragScrollComponent,
        ParamItemComponent,
        SchemeSectionComponent,
        ParamsDialogComponent,
        DocComponent,
        StatusManageItemComponent,
        HelpItemComponent,
        NoSanitizePipe,
        SectionDetailDialogComponent,
        DeviceItemGroupDetailDialogComponent,
        DeviceItemDetailDialogComponent,
        ElementsComponent,
        ManageDevicesComponent,
        ParamTypeFormComponent,
        DeviceDetailDialogComponent,
        DeviceItemTypeDetailDialogComponent,
        DeviceItemGroupTypeDetailDialogComponent,
        SignTypeDetailDialogComponent,
        PluginDetailDialogComponent,
        ElementsMenuComponent,
        MnemoschemeComponent,
        DeviceItemGroupComponent,
        LogSidebarComponent,
    ],
    exports: [
        ParamComponent
    ],
    providers: [
        SchemeLoadGuard,
        ControlService,
        SchemeService,
        SettingsService,
        SidebarService,
        Title,
    ]
})
export class SchemeModule {
}
