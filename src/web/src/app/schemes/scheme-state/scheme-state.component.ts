import { ChangeDetectorRef, Component, DoCheck, Input, IterableDiffer, IterableDiffers, KeyValueDiffer, KeyValueDiffers, inject } from '@angular/core';
import {Connection_State, Scheme_Message} from '../../user';
import {TranslateService} from '@ngx-translate/core';

import { MatTooltip } from '@angular/material/tooltip';
import { MatIcon } from '@angular/material/icon';

@Component({
    selector: 'app-scheme-state',
    templateUrl: './scheme-state.component.html',
    styleUrls: ['./scheme-state.component.css', '../schemes-list.css'],
    standalone: true,
    imports: [MatTooltip, MatIcon]
})
export class SchemeStateComponent implements DoCheck {
    private translate = inject(TranslateService);
    private differ_ = inject(KeyValueDiffers);
    private itDiffer_ = inject(IterableDiffers);
    private changeDetectorRef = inject(ChangeDetectorRef);

    status_class = {
        '1': 'ok',
        '2': 'undef',
        '3': 'warn',
        '4': 'err'
    };

    isModalOpen = false;

    get most_bad_status() {
        return this.messages?.reduce((acc, cur) => cur.status > acc ? cur.status : acc, 1);
    }

    get isSchemeConnected(): boolean {
        switch (this.connect_state & 0b111) {
            case Connection_State.CS_CONNECTED:
            case Connection_State.CS_CONNECTED_JUST_NOW:
            case Connection_State.CS_CONNECTED_SYNC_TIMEOUT:
                return true;
        }
        return false;
    }

    @Input() status_checked: boolean;
    @Input() loses_state: boolean;
    @Input() mod_state: boolean;
    @Input() connect_state: Connection_State;

    @Input() messages: Scheme_Message[];
    private messagesDiffer_: IterableDiffer<Scheme_Message>;
    private messagesDiffers_: Map<number, KeyValueDiffer<string, any>> = new Map();

    constructor() {
        this.messagesDiffer_ = this.itDiffer_.find([]).create();
    }

    ngDoCheck() {
        let apply = false;
        const changes = this.messagesDiffer_.diff(this.messages);
        if (changes) {
            changes.forEachAddedItem((item) => {
                if (!this.messagesDiffers_.has(item.currentIndex)) {
                    this.messagesDiffers_.set(item.currentIndex, this.differ_.find(item.item).create());
                }
            });

            changes.forEachRemovedItem((item) => {
               if (this.messagesDiffers_.has(item.previousIndex)) {
                   this.messagesDiffers_.delete(item.previousIndex);
               }
            });
            apply = true;
        }

        for (let [key, differ] of this.messagesDiffers_) {
            const changes = differ.diff(this.messages[key]);
            apply ||= !!changes;
        }

        if (apply) {
            this.changeDetectorRef.detectChanges();
        }
    }

    toggleModal(e: any) {
        this.isModalOpen = !this.isModalOpen;
        e.stopPropagation();
    }

    public status_desc(): string {
        let result = '';

        if (this.status_checked) {
            switch (this.connect_state & 0b111) {
                case Connection_State.CS_SERVER_DOWN:
                    result = this.translate.instant('SERVER_DOWN');
                    break;
                case Connection_State.CS_DISCONNECTED:
                    result = this.translate.instant('OFFLINE');
                    break;
                case Connection_State.CS_CONNECTED:
                    result = this.translate.instant('ONLINE');
                    break;
                case Connection_State.CS_DISCONNECTED_JUST_NOW:
                    result = this.translate.instant('DISCONNECTED_JUST_NOW');
                    break;
                case Connection_State.CS_CONNECTED_JUST_NOW:
                    result = this.translate.instant('CONNECTED_JUST_NOW');
                    break;
                case Connection_State.CS_CONNECTED_SYNC_TIMEOUT:
                    result = this.translate.instant('CONNECTED_SYNC_TIMEOUT');
                    break;
            }

            result += '. ';

            if (this.loses_state) {
                result += 'С потерями пакетов. '; // TODO: translation
            }

            if (this.mod_state) {
                result += this.translate.instant('MODIFIED') + '. ';
            }

            return result;
        }
        return this.translate.instant('WAIT') + '...';
    }

    public get_status_class(): string {
        if (!this.status_checked) {
            return 'status_check';
        }

        if (this.mod_state) {
            return 'status_modified';
        }

        switch (this.connect_state) {
            case Connection_State.CS_CONNECTED_SYNC_TIMEOUT:
                return 'status_sync_fail';
            case Connection_State.CS_CONNECTED_MODIFIED:
                return 'status_modified';
            case Connection_State.CS_DISCONNECTED_JUST_NOW:
                return 'status_bad_just';
            case Connection_State.CS_CONNECTED_JUST_NOW:
                return 'status_sync';
        }
    }
}
