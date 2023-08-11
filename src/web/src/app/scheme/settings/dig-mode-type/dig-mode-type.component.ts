import {Component} from '@angular/core';
import {DIG_Mode_Type, DIG_Type} from '../../scheme';
import {ChangeTemplate, Structure_Type} from '../settings';
import {SchemeService} from '../../scheme.service';
import {UIService} from '../../../ui.service';

@Component({
    selector: 'app-dig-mode-type',
    templateUrl: './dig-mode-type.component.html',
    styleUrls: ['../settings.css', './dig-mode-type.component.css']
})
export class DigModeTypeComponent extends ChangeTemplate<DIG_Mode_Type> {
    groupTypes: DIG_Type[];

    constructor(schemeService: SchemeService, ui: UIService) {
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
