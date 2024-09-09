import { CommonModule } from '@angular/common';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { AngularMultiSelectModule, AngularMultiSelect } from 'angular2-multiselect-dropdown';

@NgModule({
	imports: [
		CommonModule,
		FormsModule,
		AngularMultiSelectModule,
	],
	declarations: [
	],
	exports: [AngularMultiSelect],
})
export class AppModule {}
