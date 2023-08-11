import {Component, ComponentFactoryResolver, ComponentRef, OnInit, ViewContainerRef} from '@angular/core';
import {NeedSidebar} from '../sidebar.service';
import {ReportsMenuComponent} from './reports-menu/reports-menu.component';

@Component({
  selector: 'app-reports',
  templateUrl: './reports.component.html',
  styleUrls: ['./reports.component.css']
})
export class ReportsComponent implements OnInit, NeedSidebar {
    private menuRef: ComponentRef<ReportsMenuComponent>;

    constructor(private resolver: ComponentFactoryResolver) {}

    ngOnInit() {
    }

    getSidebarWidget(viewContainerRef: ViewContainerRef): ComponentRef<any> {
        const factory = this.resolver.resolveComponentFactory(ReportsMenuComponent);
        this.menuRef = viewContainerRef.createComponent(factory);

        return this.menuRef;
    }
}
