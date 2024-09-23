import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { CommonModule } from '@angular/common';



import { SettingsRoutingModule } from './settings-routing.module';

import { SettingsComponent, Scheme_Copy_Dialog } from './settings.component';
import { DevicesComponent, DeviceItemsComponent } from './devices/devices.component';
import { SectionsComponent, GroupsComponent, ParamsInGroupComponent } from './sections/sections.component';
import { GroupTypesComponent, ItemTypesComponent, ParamTypesComponent, StatusesComponent } from './group-types/group-types.component';
import { DIG_Status_Category_Component } from './dig-status-category/dig-status-category.component';
import { SignTypesComponent } from './sign-types/sign-types.component';

import { Code_Item_Component } from './codes/codes.component';
import { EditorComponent } from './codes/editor/editor.component';
import { CompleterService } from './codes/services/completer.service';
import { MetadataService } from './codes/services/metadata.service';

import { PluginTypesComponent } from './plugin-types/plugin-types.component';
import { SaveTimersComponent } from './save-timers/save-timers.component';
import { AceEditorModule } from 'ng2-ace-editor';
import { DigModeTypeComponent } from './dig-mode-type/dig-mode-type.component';
import { TranslationComponent } from './translation/translation.component';
import { ValueViewComponent } from './value-view/value-view.component';
import {ConfirmUnsavedChangesDialogComponent} from './confirm-unsaved-changes-dialog/confirm-unsaved-changes-dialog.component';
import {PendingChangesGuard} from './pending-changes.guard';

@NgModule({
    imports: [
    CommonModule,
    FormsModule,
    ReactiveFormsModule,
    SettingsRoutingModule,
    AceEditorModule,
],
    declarations: [
        SettingsComponent,
        DevicesComponent,
        DeviceItemsComponent,
        SectionsComponent,
        GroupsComponent, ParamsInGroupComponent,
        GroupTypesComponent, ItemTypesComponent, ParamTypesComponent, StatusesComponent,
        DIG_Status_Category_Component,
        SignTypesComponent,
        Code_Item_Component,
        EditorComponent,
        PluginTypesComponent,
        SaveTimersComponent,
        Scheme_Copy_Dialog,
        DigModeTypeComponent,
        TranslationComponent,
        ValueViewComponent,
        ConfirmUnsavedChangesDialogComponent,
    ],
    providers: [
        //    SchemeLoadGuard,
        //    ControlService,
        PendingChangesGuard,
        MetadataService,
        CompleterService,
    ]
})
export class SettingsModule { }
