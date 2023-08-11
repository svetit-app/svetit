import {UIService} from '../ui.service';
import {SchemeService} from './scheme.service';
import {Device_Item_Group, DIG_Param, DIG_Param_Type} from './scheme';
import {ChangeInfo, ChangeState, Structure_Type} from './settings/settings';

export function addAllGroupParamsToDig(
    ui: UIService, schemeService: SchemeService,
    groupId: Device_Item_Group['id'], groupTypeId: Device_Item_Group['type_id']
) {
    ui.confirmationDialog('Добавить все уставки из типа контура?') // TODO: i18n
        .subscribe((confirmation) => {
            if (!confirmation) return;

            addAllGroupParamsToDigImpl(schemeService, groupId, groupTypeId)
                .subscribe(() => {});
        });
}

export function addAllGroupParamsToDigImpl(schemeService: SchemeService, groupId: Device_Item_Group['id'], groupTypeId: Device_Item_Group['type_id']) {
    const paramTypes = schemeService.scheme.dig_param_type
        .filter((paramType) => paramType.group_type_id === groupTypeId && !paramType.parent_id);

    return addParamsToDigImpl(schemeService, [groupId], paramTypes);
}

export function addParamsToGroups(schemeService: SchemeService, groupTypeId: Device_Item_Group['type_id'], paramTypesIds: DIG_Param_Type['id'][]) {
    const types = schemeService.scheme.dig_param_type.filter(t => paramTypesIds.includes(t.id));
    const groupsIds: Device_Item_Group['id'][] = [];
    for (let s of schemeService.scheme.section) {
        for (let g of s.groups) {
            if (g.type_id === groupTypeId) {
                groupsIds.push(g.id);
            }
        }
    }

    return addParamsToDigImpl(schemeService, groupsIds, types);
}

export function addParamsToDigImpl(schemeService: SchemeService, groupsIds:  Device_Item_Group['type_id'][], paramTypes: DIG_Param_Type[]) {
    let groups: Device_Item_Group[] = [];
    for (let s of schemeService.scheme.section) {
        for (let g of s.groups) {
            if (groupsIds.includes(g.id)) {
                groups.push(g);
            }
        }
    }

    const params = [];
    groups.forEach((group) => mapParamTypes(group, paramTypes, params));

    return schemeService.modify_structure(Structure_Type.ST_DIG_PARAM, params);
}

function mapParamTypes(group: Device_Item_Group, paramTypes: DIG_Param_Type[], arr: ChangeInfo<DIG_Param>[] = []) {
    paramTypes.forEach((paramType) => {
        const param = new DIG_Param();
        param.param = paramType;
        param.param_id = paramType.id;
        param.group_id = group.id;
        param.childs = [];

        arr.push({
            obj: param,
            prev: null,
            state: ChangeState.Upsert,
        });

        if (paramType.childs) {
            mapParamTypes(group, paramType.childs, arr);
        }
    });
}
