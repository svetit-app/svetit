import {Patch_Structure_Response, SchemeService} from '../scheme.service';
import {UIService} from '../../ui.service';
import {Observable, isObservable} from "rxjs";

import {ComponentCanDeactivate} from './pending-changes.guard';
import {filter, map, switchMap, tap} from 'rxjs/operators';

export enum Structure_Type {
    ST_UNKNOWN,
    ST_DEVICE = 'device',
    ST_PLUGIN_TYPE = 'plugin_type',
    ST_DEVICE_ITEM = 'device_item',
    ST_DEVICE_ITEM_TYPE = 'device_item_type',
    ST_SAVE_TIMER = 'save_timer',
    ST_SECTION = 'section',
    ST_DEVICE_ITEM_GROUP = 'device_item_group',
    ST_DIG_TYPE = 'dig_type',
    ST_DIG_MODE_TYPE = 'dig_mode_type',
    ST_DIG_PARAM_TYPE = 'dig_param_type',
    ST_DIG_STATUS_TYPE = 'dig_status_type',
    ST_DIG_STATUS_CATEGORY = 'dig_status_category',
    ST_DIG_PARAM = 'dig_param',
    ST_SIGN_TYPE = 'sign_type',
    ST_CODE_ITEM = 'code_item',
    ST_TRANSLATION = 'translation',
    ST_NODE = '',
    ST_DISABLED_PARAM = '',
    ST_DISABLED_STATUS = '',
    ST_CHART = '',
    ST_CHART_ITEM = '',
    ST_VALUE_VIEW = 'value_view',
    ST_AUTH_GROUP = '',
    ST_AUTH_GROUP_PERMISSION = '',
    ST_USER = '',
    ST_USER_GROUP = '',

    // Часто изменяемые
    ST_DEVICE_ITEM_VALUE = '',
    ST_DIG_MODE = '',
    ST_DIG_PARAM_VALUE = '',
}

export enum ChangeState {
    NoChange,
    Upsert,
    Delete,
}

export interface ChangeInfo<T> {
    state: ChangeState;
    obj: T;
    prev?: T;
}

export abstract class ChangeTemplate<T extends { id: number }> implements ComponentCanDeactivate {
    changeState = ChangeState;
    changed: boolean;

    items: ChangeInfo<T>[] = [];
    sel_item: ChangeInfo<T>;

    protected constructor(
        public schemeService: SchemeService,
        private itemType: new () => T,
        private settingName: Structure_Type,
        protected ui: UIService,
    ) {
    }

    abstract getObjects(): Observable<T[]> | T[];

    fillItems(): void {
        this.changed = false;
        this.items = [];
        const objects: Observable<T[]> | T[] = this.getObjects();
        if (isObservable(objects)) {
            objects.subscribe(result => this._fillItems(result));
        } else {
            this._fillItems(objects);
        }
    }

    select(item: ChangeInfo<T>): void {
        this.sel_item = (this.sel_item === item) ? undefined : item;
    }

    itemChanged(item: ChangeInfo<T> = undefined, state: ChangeState = ChangeState.Upsert): void {
        if (item === undefined) {
            item = this.sel_item;
        }
        if (item.state !== state) {
            item.state = state;
            if (!this.changed) {
                this.changed = true;
            }
        }
    }

    public save(event: any = undefined): void {
        this.saveImpl(event).subscribe(() => {});
    }

    protected saveImpl(event: any = undefined) {
        if (event !== undefined) {
            event.stopPropagation();
        }

        return this.ui.confirmationDialog()
            .pipe(
                filter((confirmed) => confirmed),
                switchMap(() => this.saveSettings()),
            )
            .pipe(
                tap(() => {
                    this.sel_item = null;
                    this.fillItems();
                }),
            );
    }

    cancel(evnt: any = undefined): void {
        if (evnt !== undefined) {
            evnt.stopPropagation();
        }
        if (this.sel_item !== undefined) {
            this.select(this.sel_item);
        }
        this.fillItems();
    }

    initItem(obj: T): void {
    }

    create(): void {
        this.changed = true;
        let obj: T = new this.itemType();
        (<any>obj).id = 0;
        this.initItem(obj);
        this.addItem(obj);
    }

    addItem(obj: T, select: boolean = true): void {
        let item = {state: ChangeState.NoChange, obj: obj} as ChangeInfo<T>;
        this.items.push(item);
        if (select) {
            this.sel_item = item;
        }
    }

    remove(item: ChangeInfo<T>): void {
        this.itemChanged(item, ChangeState.Delete);
        // Dialog
    }

    saveSettings(items: ChangeInfo<T>[] = this.items) {
        return this.schemeService.modify_structure(this.settingName, items);
    }

    resetChanges(): void {
        this.fillItems();
    }

    canDeactivate(): boolean {
        return !this.changed;
    }

    private _fillItems(objects: T[]) {
        for (let obj of objects) {
            this.addItem(Object.assign({}, obj), false);
        }
    }
}
