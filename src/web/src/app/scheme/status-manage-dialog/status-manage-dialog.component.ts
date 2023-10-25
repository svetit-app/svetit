import { ChangeDetectorRef, Component, OnInit, OnDestroy, Inject } from '@angular/core';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

import { MediaMatcher } from '@angular/cdk/layout';

import { Device_Item_Group, DIG_Status_Type, Disabled_Status } from '../scheme';
import { SchemeService } from '../scheme.service';
import { SchemesService } from '../../schemes/schemes.service';
import { Auth_Group } from '../../user';

export enum ItemState { Init, Removed, Added, Adding }
export class Item {
    title: string;
    state: number;
    disabled: Disabled_Status;
}

@Component({
  selector: 'app-status-manage-dialog',
  templateUrl: './status-manage-dialog.component.html',
  styleUrls: ['./status-manage-dialog.component.css']
})
export class StatusManageDialogComponent implements OnInit, OnDestroy {
    mobileQuery: MediaQueryList;
    private _mobileQueryListener: () => void;

    name = "Test";
    value = 5;

    showCommon = false;
    saving = false;

    displayedColumns = ['id', 'text', 'category', 'type', 'block'];
    _types: DIG_Status_Type[];
    types: DIG_Status_Type[];

    authGroups: Auth_Group[];
    disabled: Disabled_Status[];
    items = {};

    get isLoading(): boolean {
        return this.disabled === undefined || this.authGroups === undefined || this.saving;
    }

    constructor(
        public dialogRef: MatDialogRef<StatusManageDialogComponent>,
        @Inject(MAT_DIALOG_DATA) public group: Device_Item_Group,
        public schemeService: SchemeService,
        private schemesService: SchemesService,
        changeDetectorRef: ChangeDetectorRef, media: MediaMatcher
    ) 
    {
        this.mobileQuery = media.matchMedia('(max-width: 600px)');
        this._mobileQueryListener = () => {
            changeDetectorRef.detectChanges();
            this.onoffColumns();
        };
        this.mobileQuery.addListener(this._mobileQueryListener);
    }

    ngOnInit(): void {
        this.schemesService.getAuthGroups().subscribe(items => { this.authGroups = items; this.prepareMap(); });
        this.schemeService.getDisabledStatuses(this.group.id).subscribe(items => { this.disabled = items; this.prepareMap(); });

        this._types = this.schemeService.scheme.dig_status_type.filter(type => type.group_type_id === null || type.group_type_id === this.group.type_id);
        this._types = this._types.sort((t1, t2) => t1.group_type_id < t2.group_type_id ? -1 :
                                                 t1.group_type_id > t2.group_type_id ? 1 : 0);
        this._types.unshift({ id: 0, group_type_id: null, text: 'Подключен/Отключен', category: { title: 'Инфо', color: 'blue' }, inform: true } as DIG_Status_Type);
        // TODO: Может быть заблокирован для какой то Auth_Group или для всех. Для какой то DIG или для всех. Для текущей схемы или для управляющей.
        // Если заблокированно для управляющей схемы, то остальное не актуально, тут максимальный приоритет.
        // Варианты (столбец Блокировка):
        // Нет
        // По управляющей схеме
        // Для этой группы
        // Для всех групп
        // Для этой группы у "Admins"
        // Для всех групп у "Users"
        
        this.onoffColumns();
    }

    ngOnDestroy() {
        this.mobileQuery.removeListener(this._mobileQueryListener);
    }

    onNoClick(): void {
        this.dialogRef.close();
    }

    onoffColumns() {
        if (this.mobileQuery.matches)
        {
            if (this.displayedColumns.length >= 4)
            {
                this.displayedColumns.splice(2, 1);
                this.displayedColumns.splice(0, 1);
            }
        }
        else
        {
            if (this.displayedColumns.length < 4)
            {
                this.displayedColumns.splice(0, 0, "id");
                this.displayedColumns.splice(2, 0, "category");
            }
        }

        this.showCommonToggle();
    }

    showCommonToggle(): void {
        this.types = this._types.filter(type => type.group_type_id !== null || this.showCommon);
        if (this.showCommon && !this.mobileQuery.matches && this.displayedColumns.length < 5)
            this.displayedColumns.splice(3, 0, 'type');
        else
        {
            const i = this.displayedColumns.indexOf('type');
            if (i !== -1)
                this.displayedColumns.splice(i, 1);
        }
    }

    prepareMap(): void
    {
        if (this.isLoading)
            return;

        for (const type of this._types)
            this.items[type.id] = [];

        for (const disabled of this.disabled)
        {
            let arr = this.items[disabled.status_id ? disabled.status_id : 0];
            if (!arr)
                continue;

            if (disabled.scheme_id !== this.schemeService.scheme.id)
            {
                this.items[disabled.status_id ? disabled.status_id : 0] = null; // "По управляющей схеме";
                continue;
            }

            if (disabled.dig_id != null && disabled.dig_id != this.group.id)
                continue;

            const title = this.getTitle(disabled);
            const item = { title, state: ItemState.Init, disabled } as Item;
            arr.push(item);
        }
    }

    getTitle(disabled: Disabled_Status): string
    {
        let item = disabled.dig_id == this.group.id ? "Для этой группы" : "Для всех групп";
        if (disabled.group_id)
            item += ' у "' + this.getAuthGroupName(disabled.group_id) + '"';
        return item;
    }

    getAuthGroupName(id: number): string
    {
        for (const group of this.authGroups)
            if (group.id == id)
                return group.name;
        return "Unknown";
    }

    save()
    {
        this.saving = true;

        let addList = [];
        let delList = [];

        for (const i in this.items)
        {
            const statusIt = this.items[i];
            if (!statusIt)
                continue;
            for (const item of statusIt)
            {
                if (item.state == ItemState.Added)
                    addList.push(item.disabled);
                else if (item.state == ItemState.Removed)
                    delList.push(item.disabled);
            }
        }

        if (delList.length)
            this.schemeService.delDisabledStatuses(delList).subscribe(() => { this.authGroups = undefined; this.checkClose() });
        else
            this.authGroups = undefined;

        if (addList.length)
            this.schemeService.addDisabledStatuses(addList).subscribe(() => { this.disabled = undefined; this.checkClose() });
        else
            this.disabled = undefined;

        this.checkClose();
    }

    checkClose()
    {
        if (!this.authGroups && !this.disabled)
            this.dialogRef.close();
    }
}
