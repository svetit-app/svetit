import {Component, ComponentRef, Injectable, ViewContainerRef} from '@angular/core';
import {Observable} from 'rxjs/Observable';
import {Subject} from 'rxjs';

export interface NeedSidebar {
    getSidebarWidget(viewContainerRef: ViewContainerRef): ComponentRef<any>;
}

export function needSidebarHelper(component: Component | (Component & NeedSidebar)): boolean {
    return 'getSidebarWidget' in component;
}

export interface SidebarAction<T> {
    type: string;
    data: T;
}

@Injectable()
export class SidebarService {
    private _sidebarEvents: Subject<SidebarAction<any>>;
    private _contentActions: Subject<SidebarAction<any>>;

    constructor() {
        this.resetContent();
        this.resetSidebar();
    }


    getSidebarActionBroadcast(): Observable<SidebarAction<any>> {
        return this._sidebarEvents.asObservable();
    }

    getContentActionBroadcast(): Observable<SidebarAction<any>> {
        return this._contentActions.asObservable();
    }

    performActionToContent<T>(action: SidebarAction<T>) {
        this._contentActions.next(action);
    }

    performActionToSidebar<T>(action: SidebarAction<T>) {
        this._sidebarEvents.next(action);
    }

    resetSidebar(): void {
        this._sidebarEvents = new Subject();
    }

    resetContent(): void {
        this._contentActions = new Subject();
    }
}
