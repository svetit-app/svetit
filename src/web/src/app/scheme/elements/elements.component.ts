import { Component, OnInit, ComponentFactoryResolver, ComponentRef, ViewContainerRef } from '@angular/core';

import {NeedSidebar} from '../sidebar.service';
import {ElementsMenuComponent} from './elements-menu/elements-menu.component';

@Component({
  selector: 'app-elements',
  templateUrl: './elements.component.html',
  styleUrls: ['./elements.component.css']
})
export class ElementsComponent implements OnInit, NeedSidebar {

    constructor(
        private resolver: ComponentFactoryResolver,
    ) { }

    ngOnInit(): void {
    }

    getSidebarWidget(viewContainerRef: ViewContainerRef): ComponentRef<any> {
        const factory = this.resolver.resolveComponentFactory(ElementsMenuComponent);
        return viewContainerRef.createComponent(factory);
    }
}
