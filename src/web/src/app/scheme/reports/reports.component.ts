import { Component, ComponentFactoryResolver, ComponentRef, OnInit, ViewContainerRef, inject } from '@angular/core';
import {NeedSidebar} from '../sidebar.service';
import {ReportsMenuComponent} from './reports-menu/reports-menu.component';
import { RouterOutlet } from '@angular/router';

@Component({
    selector: 'app-reports',
    templateUrl: './reports.component.html',
    styleUrls: ['./reports.component.css'],
    standalone: true,
    imports: [RouterOutlet]
})
export class ReportsComponent implements OnInit, NeedSidebar {
    private resolver = inject(ComponentFactoryResolver);

    private menuRef: ComponentRef<ReportsMenuComponent>;

    ngOnInit() {
    }

    getSidebarWidget(viewContainerRef: ViewContainerRef): ComponentRef<any> {
        const factory = this.resolver.resolveComponentFactory(ReportsMenuComponent);
        this.menuRef = viewContainerRef.createComponent(factory);

        return this.menuRef;
    }
}
