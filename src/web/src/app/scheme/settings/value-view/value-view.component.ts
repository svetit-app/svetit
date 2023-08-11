import {Component, OnInit} from '@angular/core';
import {Device_Item_Type, Value_View, Value_View_Detail} from '../../scheme';
import {ChangeTemplate, Structure_Type} from '../settings';
import {SchemeService} from '../../scheme.service';
import {SettingsService} from '../../settings.service';
import {UIService} from '../../../ui.service';

@Component({
    selector: 'app-value-view',
    templateUrl: './value-view.component.html',
    styleUrls: ['./value-view.component.css', '../settings.css']
})
export class ValueViewComponent extends ChangeTemplate<Value_View_Detail & { type?: Device_Item_Type }> {
    value_views: (Value_View_Detail & { type?: Device_Item_Type })[];
    types: Device_Item_Type[];

    constructor(schemeService: SchemeService, private settingsService: SettingsService, ui: UIService) {
        super(schemeService, Value_View_Detail, Structure_Type.ST_VALUE_VIEW, ui);

        this.types = this.schemeService.scheme.device_item_type;
        this.settingsService.getValueViewsDetail().subscribe(d => {
            this.value_views = d;
            this.value_views.forEach((vv) => this.updateType(vv));
            this.fillItems();
        });
    }

    getObjects(): Value_View_Detail[] {
        return this.value_views;
    }

    updateType(item: Value_View_Detail & { type?: Device_Item_Type }) {
        item.type = this.types.find(t => t.id === item.type_id)
    }
}
