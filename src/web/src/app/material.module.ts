import { NgModule } from '@angular/core';

import { MatLegacyButtonModule as MatButtonModule } from '@angular/material/legacy-button';
import {
//	MatButtonToggleModule,
MatLegacyCardModule as MatCardModule } from '@angular/material/legacy-card';
import { MatLegacyCheckboxModule as MatCheckboxModule } from '@angular/material/legacy-checkbox';
import { MatNativeDateModule } from '@angular/material/core';
import { MatDatepickerModule } from '@angular/material/datepicker';
import { MatLegacyDialogModule as MatDialogModule } from '@angular/material/legacy-dialog';
import {
//	MatChipsModule,
//	MatDividerModule,
MatExpansionModule } from '@angular/material/expansion';
import { MatIconModule } from '@angular/material/icon';
import { MatLegacyInputModule as MatInputModule } from '@angular/material/legacy-input';
import {
//	MatGridListModule,
MatLegacyListModule as MatListModule } from '@angular/material/legacy-list';
import { MatLegacyPaginatorModule as MatPaginatorModule } from '@angular/material/legacy-paginator';
import { MatLegacyProgressSpinnerModule as MatProgressSpinnerModule } from '@angular/material/legacy-progress-spinner';
import { MatLegacyRadioModule as MatRadioModule } from '@angular/material/legacy-radio';
import { MatLegacySelectModule as MatSelectModule } from '@angular/material/legacy-select';
import {
//	MatMenuModule,
//	MatProgressBarModule,
//	MatRippleModule,
MatSidenavModule } from '@angular/material/sidenav';
import { MatLegacySlideToggleModule as MatSlideToggleModule } from '@angular/material/legacy-slide-toggle';
import { MatLegacySliderModule as MatSliderModule } from '@angular/material/legacy-slider';
import { MatLegacySnackBarModule as MatSnackBarModule } from '@angular/material/legacy-snack-bar';
import { MatSortModule } from '@angular/material/sort';
import { MatStepperModule } from '@angular/material/stepper';
import { MatLegacyTableModule as MatTableModule } from '@angular/material/legacy-table';
import {
//	MatTabsModule,
MatToolbarModule } from '@angular/material/toolbar';
import { MatLegacyTooltipModule as MatTooltipModule } from '@angular/material/legacy-tooltip';
import { CdkTableModule } from '@angular/cdk/table';
import { MatLegacyAutocompleteModule} from '@angular/material/legacy-autocomplete';

@NgModule({
	exports: [
		MatButtonModule,
		MatTooltipModule,
		MatSlideToggleModule,
		MatSliderModule,
		MatSelectModule,
		MatInputModule,
		MatDialogModule,
		MatTableModule,
		MatSnackBarModule,

		MatProgressSpinnerModule,
		MatPaginatorModule,
		MatSortModule,
		MatDatepickerModule,
		MatNativeDateModule,
		MatRadioModule,
		MatIconModule,
		MatToolbarModule,
		MatSidenavModule,
		MatListModule,

//		LayoutModule,
//	MatAutocompleteModule,
//	MatButtonToggleModule,
	MatCardModule,
	MatCheckboxModule,
//	MatChipsModule,
//	MatDividerModule,
	MatExpansionModule,
//	MatGridListModule,
//	MatMenuModule,
//	MatProgressBarModule,
//	MatRippleModule,
//	MatSnackBarModule,
	MatStepperModule,
//	MatTabsModule,
	MatLegacyAutocompleteModule
	],
	declarations: [],
})
export class MaterialModule {}
