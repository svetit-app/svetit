import {Component, OnInit} from '@angular/core';
import {ChangeInfo, ChangeTemplate, Structure_Type} from '../settings';
import {SchemeService} from '../../scheme.service';
import {SettingsService} from '../../settings.service';
import {Translation} from '../../scheme';
import {UIService} from '../../../ui.service';

interface TranslationData {
    section: Array<{ id: number, name: string }>;
    device: Array<{ id: number, name: string }>;
    device_item: Array<{ id: number, name: string }>;
    dig_param_type: Array<{ id: number, title: string, description: string }>;
    dig_type: Array<{ id: number, title: string, description: string }>;
    dig_mode_type: Array<{ id: number, title: string }>;
    device_item_type: Array<{ id: number, title: string }>;
    sign_type: Array<{ id: number, name: string }>;
    dig_status_category: Array<{ id: number, title: string }>;
    dig_status_type: Array<{ id: number, text: string }>;
    value_view: Array<{ type_id: number, view: string }>;
}

const idName = {
    id: 'number',
    name: 'string',
};

const idTitle = {
    id: 'number',
    title: 'string',
};

const idTitleDesc = {
    ...idTitle,
    description: 'string',
};

@Component({
    selector: 'app-translation',
    templateUrl: './translation.component.html',
    styleUrls: ['../settings.css', './translation.component.css']
})
export class TranslationComponent extends ChangeTemplate<Translation> implements OnInit {
    readonly translationDataDescription: Record<keyof TranslationData, Record<string, string>> = {
        section: idName,
        device: idName,
        device_item: idName,
        sign_type: idName,

        dig_param_type: idTitleDesc,
        dig_type: idTitleDesc,

        dig_status_category: idTitle,
        dig_mode_type: idTitle,
        device_item_type: idTitle,

        dig_status_type: { id: 'number', text: 'string' },
        value_view: { type_id: 'number', view: 'string' },
    };

    translations: Translation[];
    translationData: TranslationData;
    selectedTranslationData: keyof TranslationData;

    keys = Object.keys;

    constructor(schemeService: SchemeService, private settings: SettingsService, ui: UIService) {
        super(schemeService, Translation, Structure_Type.ST_TRANSLATION, ui);
    }

    ngOnInit(): void {
        this.settings.getTranslations().subscribe(translation => {
            this.translations = translation;
            this.fillItems();
        });
    }

    getObjects(): any[] {
        return this.translations;
    }

    select(item: ChangeInfo<Translation>) {
        if (this.sel_item?.obj && this.translationData) {
            this.sel_item.obj.data = JSON.stringify(this.translationData);
        }

        super.select(item);
        this.translationData = this.sel_item ? JSON.parse(this.sel_item.obj.data) : undefined;
    }

    save(evnt: any = undefined) {
        if (this.sel_item?.obj && this.translationData) {
            this.sel_item.obj.data = JSON.stringify(this.translationData);
        }

        super.save(evnt);
    }

    selectTranslation(selectedKey: string) {
        const key = selectedKey as keyof TranslationData;
        this.selectedTranslationData = (key === this.selectedTranslationData) ? undefined : key;
    }

    removeLanguageItem($index: number) {
        this.translationData[this.selectedTranslationData].splice($index, 1);
        super.itemChanged();
    }

    newLanguageItem() {
        this.translationData[this.selectedTranslationData].push({} as any);
    }
}
