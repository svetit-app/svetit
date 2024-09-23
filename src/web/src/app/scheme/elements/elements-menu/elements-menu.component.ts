import { Component, OnInit, inject } from '@angular/core';
import {AuthService} from '../../../auth/service';
import {SidebarService} from '../../sidebar.service';
import {ControlService} from '../../control.service';
import { RouterLink, RouterLinkActive } from '@angular/router';

import { MatSlideToggle } from '@angular/material/slide-toggle';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { MatButton } from '@angular/material/button';
import { MatIcon } from '@angular/material/icon';

@Component({
    selector: 'app-elements-menu',
    templateUrl: './elements-menu.component.html',
    styleUrls: ['./elements-menu.component.css'],
    standalone: true,
    imports: [RouterLink, RouterLinkActive, MatSlideToggle, ReactiveFormsModule, FormsModule, MatButton, MatIcon]
})
export class ElementsMenuComponent implements OnInit {
    private user = inject(AuthService);
    private sidebar = inject(SidebarService);
    private controlService = inject(ControlService);

    isEditorModeEnabled = false;

    get canEditScheme(): boolean {
        return this.user.canEditScheme();
    }

    get canRestart(): boolean {
        return this.user.canRestart();
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
