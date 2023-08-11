import {Plugin_Type} from './scheme';
import {SettingsService} from './settings.service';
import {tap} from 'rxjs/operators';
import {PaginatorApi} from '../user';
import {Observable} from 'rxjs/Observable';

export abstract class WithPlugin<T extends { id: number, extra: string }> {
    plugins: Plugin_Type[];
    editingExtraFields: { title: string, value: string }[];

    public init(settingsService: SettingsService): Observable<PaginatorApi<Plugin_Type>> {
        return settingsService.getPluginTypes().pipe(
            tap(plugins => this.plugins = plugins.results),
        );
    }

    public pluginChanged(pluginId: number, extra: string | Array<string>, isItem: boolean = false) {
        this.editingExtraFields = [];
        const selectedPlugin = this.plugins?.find(p => p.id === pluginId);
        if (selectedPlugin) {
            const fields = selectedPlugin[isItem ? 'param_names_device_item' : 'param_names_device'];

            if (fields) {
                const parsedExtra = typeof extra === 'string' ? JSON.parse(extra) as Array<string> : extra;
                this.editingExtraFields = fields
                    .split('|')
                    .map((title, idx) => {
                        let value = '';
                        if (parsedExtra && parsedExtra[idx]) {
                            value = parsedExtra[idx];
                        }

                        return {
                            title,
                            value,
                        };
                    });
            }
        }
    }
}
