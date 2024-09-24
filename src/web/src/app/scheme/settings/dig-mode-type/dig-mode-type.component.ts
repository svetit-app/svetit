import { Component, inject } from '@angular/core';
import {DIG_Mode_Type, DIG_Type} from '../../scheme';
import {ChangeTemplate, Structure_Type} from '../settings';
import {SchemeService} from '../../scheme.service';
import {UIService} from '../../../ui.service';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatIconModule } from '@angular/material/icon';
import { MatFormFieldModule } from '@angular/material/form-field';
import { FormsModule } from '@angular/forms';
import { MatSelectModule } from '@angular/material/select';

@Component({
	selector: 'app-dig-mode-type',
	templateUrl: './dig-mode-type.component.html',
	styleUrls: ['../settings.css', './dig-mode-type.component.css'],
	standalone: true,
	imports: [
		FormsModule, MatExpansionModule, MatIconModule, MatFormFieldModule, MatSelectModule,
	]
})
export class DigModeTypeComponent extends ChangeTemplate<DIG_Mode_Type> {
    groupTypes: DIG_Type[];

    constructor() {
        const schemeService = inject(SchemeService);
        const ui = inject(UIService);

        super(schemeService, DIG_Mode_Type, Structure_Type.ST_DIG_MODE_TYPE, ui);
    }

    ngOnInit(): void {
        this.groupTypes = this.schemeService.scheme.dig_type;
        this.fillItems();
    }

    getObjects(): DIG_Mode_Type[] {
        return this.schemeService.scheme.dig_mode_type;
    }
}
