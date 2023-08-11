import {SidebarService} from './sidebar.service';

export class EditorModeFromSidebar {
    public isEditorModeEnabled: boolean;

    constructor(private sidebar: SidebarService) {
    }

    init() {
        this.sidebar.getContentActionBroadcast().subscribe((act) => {
            if (act.type === 'editor_mode') {
                this.isEditorModeEnabled = act.data;
            }
        });
    }
}
