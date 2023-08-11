import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';
import { RouterModule } from "@angular/router";

import { MatLegacyFormFieldModule as MatFormFieldModule } from '@angular/material/legacy-form-field';
import { MatIconModule } from '@angular/material/icon';
import { MatLegacySelectModule as MatSelectModule } from '@angular/material/legacy-select';

import { SchemeDetailComponent } from './detail/detail.component';
import { SchemeStateComponent } from './scheme-state/scheme-state.component';
import {MaterialModule} from '../material.module';
import {SchemeGroupsModule} from '../scheme-groups/scheme-groups.module';


@NgModule({
    imports: [
        CommonModule,
        FormsModule,
        RouterModule,
        MatFormFieldModule,
        MatSelectModule,
        MatIconModule,
        MaterialModule,
        ReactiveFormsModule,
        SchemeGroupsModule
    ],
  declarations: [
    SchemeDetailComponent,
    SchemeStateComponent,
  ],
  exports: [
    SchemeDetailComponent,
    SchemeStateComponent,
  ]
})
export class SchemesDetailModule { }
