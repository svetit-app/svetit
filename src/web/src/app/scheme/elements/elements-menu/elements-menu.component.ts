import {Component, OnInit} from '@angular/core';
import {AuthService} from '../../../auth/service';
import {SidebarService} from '../../sidebar.service';
import {ControlService} from '../../control.service';

@Component({
    selector: 'app-elements-menu',
    templateUrl: './elements-menu.component.html',
    styleUrls: ['./elements-menu.component.css']
})
export class ElementsMenuComponent implements OnInit {
    isEditorModeEnabled = false;

    get canEditScheme(): boolean {
        return this.user.canEditScheme();
    }

    get canRestart(): boolean {
        return this.user.canRestart();
    }

    constructor(
        private user: AuthService,
        private sidebar: SidebarService,
        private controlService: ControlService,
    ) {
    }

    ngOnInit(): void {
        this.emitEditorMode();
    }

    restart(): void {
        this.controlService.restart();
    }

    setEditorMode($event: boolean) {
        this.isEditorModeEnabled = $event;
        this.emitEditorMode();
    }

    private emitEditorMode() {
        this.sidebar.performActionToContent({
            type: 'editor_mode',
            data: this.isEditorModeEnabled,
        });
    }
}
