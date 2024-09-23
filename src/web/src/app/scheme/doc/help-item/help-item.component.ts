import { Component, OnInit, Input } from '@angular/core';

import { Help } from '../../scheme';
import { MatAccordion, MatExpansionPanel, MatExpansionPanelHeader, MatExpansionPanelTitle, MatExpansionPanelDescription } from '@angular/material/expansion';

import { NoSanitizePipe } from '../../no-sanitize.pipe';

export class HelpItem {
    help: Help;
    childs: HelpItem[];
}

@Component({
    selector: 'app-help-item',
    templateUrl: './help-item.component.html',
    styleUrls: ['./help-item.component.css'],
    standalone: true,
    imports: [MatAccordion, MatExpansionPanel, MatExpansionPanelHeader, MatExpansionPanelTitle, MatExpansionPanelDescription, NoSanitizePipe]
})
export class HelpItemComponent implements OnInit {

    @Input() item: HelpItem;

    constructor() { }

    ngOnInit(): void {
    }
}
