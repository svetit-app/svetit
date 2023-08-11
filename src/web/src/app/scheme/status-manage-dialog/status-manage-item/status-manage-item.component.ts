import { Component, OnInit, Input } from '@angular/core';

import { DIG_Status_Type, Disabled_Status } from '../../scheme';
import { SchemeService } from '../../scheme.service';

import { StatusManageDialogComponent, ItemState, Item } from '../status-manage-dialog.component';

@Component({
  selector: 'app-status-manage-item',
  templateUrl: './status-manage-item.component.html',
  styleUrls: ['./status-manage-item.component.css']
})
export class StatusManageItemComponent implements OnInit {

    @Input() statusType: DIG_Status_Type;
    @Input() manage: StatusManageDialogComponent;

    items: Item[];
    itemState = ItemState;

    constructor(
        private schemeService: SchemeService
    ) { }

    ngOnInit(): void {
        this.items = this.manage.items[this.statusType.id];
    }

    add()
    {
        this.items.push({ title: null, state: ItemState.Adding, disabled: null });
    }

    remove(item: Item)
    {
        if (item.state == ItemState.Init)
            item.state = ItemState.Removed;
        else
            this.items.splice(this.items.indexOf(item), 1);
    }

    save(item: Item, elem_id: number, auth_group_id: number)
    {
        item.disabled = { 
            group_id: auth_group_id ? auth_group_id : null, 
            dig_id: elem_id ? null : this.manage.group.id,
            status_id: this.statusType.id,
            scheme_id: this.manage.schemeService.scheme.id
        } as Disabled_Status;
        item.title = this.manage.getTitle(item.disabled);
        item.state = ItemState.Added;
    }
}
