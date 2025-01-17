import { Component, OnInit, ViewEncapsulation, inject } from '@angular/core';

import { SchemeService } from '../scheme.service';
import { Help } from '../scheme';

import { HelpItem, HelpItemComponent } from './help-item/help-item.component';

import { MatProgressSpinner } from '@angular/material/progress-spinner';
import { MatStepper, MatStepperIcon } from '@angular/material/stepper';

@Component({
    selector: 'app-doc',
    templateUrl: './doc.component.html',
    styleUrls: ['./doc.component.css'],
    encapsulation: ViewEncapsulation.None,
    standalone: true,
    imports: [MatProgressSpinner, HelpItemComponent, MatStepper, MatStepperIcon]
})
export class DocComponent implements OnInit {
    private schemeService = inject(SchemeService);


    isLoading = true;
    item: HelpItem = { help: null, childs: [] };

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
