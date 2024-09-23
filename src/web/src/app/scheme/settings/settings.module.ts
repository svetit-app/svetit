import { NgModule } from '@angular/core';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { CommonModule } from '@angular/common';



import { SettingsRoutingModule } from './settings-routing.module';

import { SettingsComponent, Scheme_Copy_Dialog } from './settings.component';
import { SectionsComponent, GroupsComponent, ParamsInGroupComponent } from './sections/sections.component';
import { GroupTypesComponent, ItemTypesComponent, ParamTypesComponent, StatusesComponent } from './group-types/group-types.component';
import { DIG_Status_Category_Component } from './dig-status-category/dig-status-category.component';
import { SignTypesComponent } from './sign-types/sign-types.component';

import { CompleterService } from './codes/services/completer.service';
import { MetadataService } from './codes/services/metadata.service';

import { PluginTypesComponent } from './plugin-types/plugin-types.component';
import { SaveTimersComponent } from './save-timers/save-timers.component';
import { TranslationComponent } from './translation/translation.component';
import { ValueViewComponent } from './value-view/value-view.component';
import {ConfirmUnsavedChangesDialogComponent} from './confirm-unsaved-changes-dialog/confirm-unsaved-changes-dialog.component';
import {PendingChangesGuard} from './pending-changes.guard';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatIconModule } from '@angular/material/icon';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatSelectModule } from '@angular/material/select';
import { MatDialogModule } from '@angular/material/dialog';

@NgModule({
    imports: [
    CommonModule,
    FormsModule,
    ReactiveFormsModule,
    SettingsRoutingModule,
	MatExpansionModule, MatIconModule, MatFormFieldModule, MatSelectModule, MatDialogModule,
],
    declarations: [
        SettingsComponent,
        SectionsComponent,
        GroupsComponent, ParamsInGroupComponent,
        GroupTypesComponent, ItemTypesComponent, ParamTypesComponent, StatusesComponent,
        DIG_Status_Category_Component,
        SignTypesComponent,
        PluginTypesComponent,
        SaveTimersComponent,
        Scheme_Copy_Dialog,
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
