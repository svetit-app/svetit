import { AfterViewInit, Component, OnInit, inject } from '@angular/core';
import {ActivatedRoute, NavigationEnd, Router} from '@angular/router';
import {MatDialog} from '@angular/material/dialog';

import {SchemeService} from '../../scheme.service';
import {Device_Item_Group, DIG_Param, DIG_Param_Type, Section} from '../../scheme';
import {AuthService} from '../../../auth/service';
import {Section_Details, SectionDetailDialogComponent} from './section-detail-dialog/section-detail-dialog.component';
import {
    Device_Item_Group_Details,
    DeviceItemGroupDetailDialogComponent
} from './device-item-group-detail-dialog/device-item-group-detail-dialog.component';
import {UIService} from '../../../ui.service';
import {ChangeInfo, ChangeState, Structure_Type} from '../../settings/settings';
import {SidebarService} from '../../sidebar.service';
import {EditorModeFromSidebar} from '../../editor-mode-from-sidebar';
import {addAllGroupParamsToDig} from '../../add-params-helpers';

import { SchemeSectionComponent } from '../../scheme-section/scheme-section.component';
import { DeviceItemGroupComponent } from '../../device-item-group/device-item-group.component';
import { MatButton } from '@angular/material/button';

@Component({
    selector: 'app-manage',
    templateUrl: './manage.component.html',
    styleUrls: ['../../../sections.css', './manage.component.css'],
    standalone: true,
    imports: [
    SchemeSectionComponent,
    DeviceItemGroupComponent,
    MatButton
],
})
export class ManageComponent extends EditorModeFromSidebar implements OnInit, AfterViewInit {
    private route = inject(ActivatedRoute);
    private schemeService = inject(SchemeService);
    private auth = inject(AuthService);
    private router = inject(Router);
    dialog = inject(MatDialog);
    private ui = inject(UIService);

    schemeName: string;
    sections: Section[] = [];

    currentSection: number;
    currentGroup: number;

    isDisabled(): boolean {
        return !this.schemeService.isSchemeConnected;
    }

    sctCount: number;

    constructor() {
        const sidebar = inject(SidebarService);

        super(sidebar);
        const router = this.router;

        router.events.subscribe(s => {
            if (s instanceof NavigationEnd) {
                const tree = router.parseUrl(router.url);
                if (tree.fragment) {
                    const parsed = tree.fragment.match(/^section-(\d+)-group-(\d+)$/);
                    if (parsed) {
                        this.currentSection = parseInt(parsed[1], 10);
                        this.currentGroup = parseInt(parsed[2], 10);

                        this.scrollToGroup(this.currentGroup);
                    }
                }
            }
        });
    }

    ngOnInit() {
        this.schemeName = this.schemeService.scheme.name;
        this.sections = this.schemeService.scheme.section;
        this.sctCount = this.sections.length;
        if (this.sctCount) {
            this.currentSection = this.sections[0].id;
        }

        super.init();
    }

    ngAfterViewInit(): void {
        this.scrollToGroup(this.currentGroup);
    }

    scrollToGroup(group_id: number) {
        const el = document.querySelector('#scheme-group-' + group_id);

        if (el) {
            setTimeout(() => {
                el.scrollIntoView({block: 'start', inline: 'center', behavior: 'smooth'});
            }, 200);
        }
    }

    newGroup(parentSection: Section) {
        this.dialog
            .open(DeviceItemGroupDetailDialogComponent, {width: '80%', data: { section_id: parentSection.id }})
            .afterClosed()
            .subscribe((createdGroup?: Device_Item_Group_Details) => {
                if (!createdGroup) return;

                addAllGroupParamsToDig(this.ui, this.schemeService, createdGroup.id, createdGroup.type_id);
            });
    }

    newSection() {
        this.dialog
            .open(SectionDetailDialogComponent, {width: '80%', data: null})
            .afterClosed()
            .subscribe((sectionDetails?: Section_Details) => {});
    }

    editSection(section: Section) {
        this.dialog
            .open(SectionDetailDialogComponent, {width: '80%', data: section})
            .afterClosed()
            .subscribe((sectionDetails?: Section_Details) => {});
    }

    removeSection(sct: Section) { // TODO: refactor remove methods below to use confirmationDialog().pipe() instead of nested subscribe
        this.ui.confirmationDialog()
            .subscribe((confirm) => {
                if (!confirm) {
                    return;
                }

                this.schemeService.remove_structure(Structure_Type.ST_SECTION, sct)
                    .subscribe(() => {});
            });
    }
}
