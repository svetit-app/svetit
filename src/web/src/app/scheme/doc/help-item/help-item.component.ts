import { Component, OnInit, Input } from '@angular/core';

import { Help } from '../../scheme';

export class HelpItem {
    help: Help;
    childs: HelpItem[];
}

@Component({
  selector: 'app-help-item',
  templateUrl: './help-item.component.html',
  styleUrls: ['./help-item.component.css']
})
export class HelpItemComponent implements OnInit {

    @Input() item: HelpItem;

    constructor() { }

    ngOnInit(): void {
    }
}
