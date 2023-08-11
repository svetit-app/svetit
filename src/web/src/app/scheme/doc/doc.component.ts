import {Component, OnInit, ViewEncapsulation} from '@angular/core';

import { SchemeService } from '../scheme.service';
import { Help } from '../scheme';

import { HelpItem } from './help-item/help-item.component';

@Component({
  selector: 'app-doc',
  templateUrl: './doc.component.html',
  styleUrls: ['./doc.component.css'],
  encapsulation: ViewEncapsulation.None
})
export class DocComponent implements OnInit {

    isLoading = true;
    item: HelpItem = { help: null, childs: [] };

    constructor(
        private schemeService: SchemeService
    ) { }

    ngOnInit() {
        this.schemeService.getHelp().subscribe(items => this.setItems(items));
    }

    setItems(items: Help[]) {
        for (const item of items)
        {
            const parentItem = item.parent_id ? this.getParent(this.item, item) : this.item;
            parentItem.childs.push({ help: item, childs: [] });
        }

        this.isLoading = false;
    }

    getParent(parentItem: HelpItem, help: Help): HelpItem
    {
        for (const it of parentItem.childs)
        {
            if (it.help.id === help.parent_id)
                return it;
            const it2 = this.getParent(it, help);
            if (it2 !== it)
                return it2;
        }

        return parentItem;
    }
}
