import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable, of } from 'rxjs';

import { SchemeService } from '../../../scheme.service';
import { MetaInfoModel } from './../models/metadata.model';

@Injectable()
export class MetadataService {

    metainfo: MetaInfoModel[] = [{
        Name: 'api',
        Description: 'Container for Das api methods',
        Type: 'class',
        Children: [{
            Name: "actDevice",
            Description: "Change device item state",
            Type: "void",
            Children: [],
            InputParameters: { group: 'Device_Item_Group', type: 'number', newState: 'any', user_id: 'number' }
        },{
            Name: "findItem",
            Description: "Items forEach",
            Type: "void",
            Children: [],
            InputParameters: { items: 'any[]', func: 'function' }
        },{
            Name: "status",
            Description: "Available DIG status map",
            Type: "Object",
            Children: []
        },{
            Name: "checker",
            Description: "Checkers array",
            Type: "Array",
            Children: []
        },{
            Name: "mng",
            Description: "Scheme manager",
            Type: "class",
            Children: [{
                Name: "sections",
                Description: "Sections array",
                Type: "Array",
                Children: []
            },{
                Name: "devices",
                Description: "Devices array",
                Type: "Array",
                Children: []
            }]
        },{
            Name: "handlers",
            Description: "Handlers map",
            Type: "Object",
            Children: [{
                Name: "changed",
                Description: "Handlers for change events",
                Type: "Object",
                Children: [{
                    Name: "mode",
                    Description: "DIG Mode changed handler",
                    Type: "void",
                    Children: [],
                    InputParameters: { group: 'Device_Item_Group', mode_id: 'number', user_id: 'number' }
                },{
                    Name: "item",
                    Description: "Device item state changed handler",
                    Type: "void",
                    Children: [],
                    InputParameters: { group: 'Device_Item_Group', item: 'Device_Item', user_id: 'number', value: 'any' }
                },{
                    Name: "sensor",
                    Description: "Device item state changed handler (sensors only)",
                    Type: "void",
                    Children: [],
                    InputParameters: { group: 'Device_Item_Group', item: 'Device_Item', user_id: 'number', value: 'any' }
                },{
                    Name: "control",
                    Description: "Device item state changed handler (controls only)",
                    Type: "void",
                    Children: [],
                    InputParameters: { group: 'Device_Item_Group', item: 'Device_Item', user_id: 'number', value: 'any' }
                },{
                    Name: "day_part",
                    Description: "Day part changed handler",
                    Type: "void",
                    Children: [],
                    InputParameters: { section: 'Section', is_day: 'boolean' }
                }],
            },{
                Name: "database",
                Description: "Handlers for database events",
                Type: "Object",
                Children: [{
                    Name: "initialized",
                    Description: "Database initialized handler",
                    Type: "void",
                    Children: [],
                    InputParameters: {}
                }]
            },{
                Name: "section",
                Description: "Handlers for sections events",
                Type: "Object",
                Children: [{
                    Name: "initialized",
                    Description: "Section initialized handler",
                    Type: "void",
                    Children: [],
                    InputParameters: { section: 'Section' }
                }]
            },{
                Name: "group",
                Description: "Handlers for DIG events",
                Type: "Object",
                Children: [{
                    Name: "initialized",
                    Description: "DIG initialized handlers map",
                    Type: "Object",
                    Children: [],// TODO: fill dynamicaly for each DIG { InputParameters: { group: 'Device_Item_Group' }}
                },{
                    Name: "changed",
                    Description: "DIG changed handlers map",
                    Type: "Object",
                    Children: [],// TODO: fill dynamicaly for each DIG { InputParameters: { group: 'Device_Item_Group', item: 'Device_Item', user_id: 'number', value: 'any' }}
                }]
            },{
                Name: "control_change_check",
                Description: "Handler for accept device item change state",
                Type: "boolean",
                Children: [],
                InputParameters: { group: 'Device_Item_Group', item: 'Device_Item', value: 'any', user_id: 'number' }
            },{
                Name: "initialized",
                Description: "Handler called after all initializations",
                Type: "void",
                Children: [],
                InputParameters: {}
            }]
        },{
            Name: 'type',
            Description: 'Types map',
            Type: 'Object',
            Children: [{
                Name: 'item',
                Description: 'Device item types map',
                Type: 'Object',
                Children: []
            },{
                Name: 'group',
                Description: 'DIG types map',
                Type: 'Object',
                Children: []
            },{
                Name: 'mode',
                Description: 'DIG modes map',
                Type: 'Object',
                Children: []
            },{
                Name: 'param',
                Description: 'DIG params map',
                Type: 'Object',
                Children: []
            }]
        }]
    }];

    constructor(
        private schemeService: SchemeService,
    ) {
        const api = this.metainfo[0].Children;
        const types = api[api.length-1].Children;
        const add_meta = (i: number, name: string, desc: string) => {
            types[i].Children.push({
                Name: name,
                Description: desc,
                Type: 'number',
                Children: []
            });
        };

        for (const t of this.schemeService.scheme.device_item_type) {
            add_meta(0, t.name, `Type ID of device item type: ${t.title}`);
        }
        for (const t of this.schemeService.scheme.dig_type) {
            add_meta(1, t.name, `Type ID of DIG type: ${t.title}`);
        }
    }

    getMetadata(): Observable<MetaInfoModel[]> {
        return of(this.metainfo);
    }
}

