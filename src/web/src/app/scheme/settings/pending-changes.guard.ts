import {Observable} from 'rxjs/Observable';
import {Injectable} from '@angular/core';
import { ActivatedRouteSnapshot, RouterStateSnapshot, UrlTree } from '@angular/router';
import {MatDialog} from '@angular/material/dialog';
import {ConfirmUnsavedChangesDialogComponent} from './confirm-unsaved-changes-dialog/confirm-unsaved-changes-dialog.component';
import {map, switchMap} from 'rxjs/operators';
import {of} from 'rxjs/observable/of';

export interface ComponentCanDeactivate {
    canDeactivate: () => boolean;
    saveSettings: () => Observable<any>;
    resetChanges: () => void;
}

@Injectable()
export class PendingChangesGuard  {
    constructor(private dialog: MatDialog) {
    }

    canDeactivate(component: ComponentCanDeactivate, currentRoute: ActivatedRouteSnapshot, currentState: RouterStateSnapshot, nextState?: RouterStateSnapshot): Observable<boolean | UrlTree> | boolean {
        const canDeactivate = component.canDeactivate();
        return canDeactivate ? true : this.dialog.open(ConfirmUnsavedChangesDialogComponent, {})
            .afterClosed()
            .pipe(switchMap((dialogResult) => {
                if (dialogResult) {
                    component.resetChanges();
                    return of(true);
                }

                return component.saveSettings().pipe(
                    map(data => true),
                );
            }));
    }
}
