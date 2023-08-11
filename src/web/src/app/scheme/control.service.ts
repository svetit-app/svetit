import {Inject, Injectable} from '@angular/core';
import {DOCUMENT} from '@angular/common';
import {Subject, SubscriptionLike} from 'rxjs';

import {SchemeService} from './scheme.service';
import {ByteMessage, ByteTools, WebSocketBytesService} from '../web-socket.service';
import {Connection_State} from '../user';
import {
    Device_Item,
    Device_Item_Group,
    DIG_Param,
    DIG_Param_Value_Type,
    DIG_Status_Type,
    Log_Base,
    Log_Event,
    Log_Mode, Log_Param,
    Log_Status,
    Log_Status_Direction,
    Time_Info,
} from './scheme';

// import { QByteArray } from 'qtdatastream/src/types';

export enum WebSockCmd {
  WS_UNKNOWN,
  WS_AUTH,
  WS_WELCOME,

  WS_CONNECTION_STATE,
  WS_WRITE_TO_DEV_ITEM,
  WS_CHANGE_GROUP_MODE,
  WS_CHANGE_GROUP_PARAM_VALUES,
  WS_EXEC_SCRIPT,
  WS_RESTART,

  WS_DEV_ITEM_VALUES,
  WS_EVENT_LOG,
  WS_GROUP_MODE,

  WS_STRUCT_MODIFY,

  WS_GROUP_STATUS_ADDED,
  WS_GROUP_STATUS_REMOVED,
  WS_TIME_INFO,
  WS_IP_ADDRESS,

  WS_STREAM_TOGGLE,
  WS_STREAM_DATA,
  WS_STREAM_TEXT,

  WS_LOG_EVENT,
  WS_LOG_MODE,
  WS_LOG_PARAM,
  WS_LOG_STATUS,
  WS_LOG_VALUE,

  WEB_SOCK_CMD_COUNT
}

export interface ConnectInfo {
  connected: boolean;
  ip: string;
  time: number;
  time_zone: string;
  modified: boolean;
}

@Injectable()
export class ControlService {
  public byte_msg: Subject<ByteMessage> = new Subject();
  public stream_msg: Subject<ByteMessage> = new Subject();
  public dev_item_changed: Subject<Device_Item[]> = new Subject();
  public group_param_values_changed: Subject<DIG_Param[]> = new Subject();

  public log_mode: Subject<Log_Mode[]> = new Subject();
  public log_status: Subject<Log_Status[]> = new Subject();
  public log_param: Subject<Log_Param[]> = new Subject();

  public opened: Subject<boolean>;

  private bmsg_sub: SubscriptionLike;

  constructor(
    private wsbService: WebSocketBytesService,
    private schemeService: SchemeService,
    @Inject(DOCUMENT) private document) {
    this.opened = wsbService.opened;
  }

  open(): void {
    this.bmsg_sub = this.wsbService.message.subscribe((msg: ByteMessage) => {
      /*
      console.log(msg);
      console.log(this.schemeService);
      */

      if (!this.schemeService.scheme || (msg.scheme_id != this.schemeService.scheme.id && msg.scheme_id != 0)) {
        return;
      }

      if (msg.cmd == WebSockCmd.WS_GROUP_MODE) {
        if (msg.data === undefined) {
          console.log('GroupMode without data');
          return;
        }

        const view = new Uint8Array(msg.data);
        const [start, mode_id] = ByteTools.parseUInt32(view);
        const [, group_id] = ByteTools.parseUInt32(view, start);

        for (const sct of this.schemeService.scheme.section) {
          for (const group of sct.groups) {
            if (group.id == group_id) {
              group.mode = mode_id;
              return;
            }
          }
        }
      } else if (msg.cmd === WebSockCmd.WS_LOG_MODE) {
          ControlService.parseLogs(msg.data, this.log_mode, Log_Mode, ControlService.logModeParser);
      } else if (msg.cmd === WebSockCmd.WS_LOG_STATUS) {
          ControlService.parseLogs(msg.data, this.log_status, Log_Status, ControlService.logStatusParser);
      } else if (msg.cmd === WebSockCmd.WS_DEV_ITEM_VALUES) {

        if (msg.data === undefined) {
          console.log('DevItemValues without data');
          return;
        }

        let dev_item_list = [];

        const view = new Uint8Array(msg.data);
        let [idx, count] = ByteTools.parseUInt32(view);
        let item_id: number;
        while (count--) {
          item_id = ByteTools.parseUInt32(view, idx)[1];
          idx += 4;

          const [last_pos1, raw_value] = ByteTools.parseQVariant(view, idx);
          idx = last_pos1;
          if (idx >= msg.data.byteLength) {
            console.log(`bad raw length ${idx} ${msg.data.byteLength} ${raw_value}`);
            break;
          }

          const [last_pos2, value] = ByteTools.parseQVariant(view, idx);
          idx = last_pos2;
          if (idx > msg.data.byteLength) {
            console.log(`bad length ${idx} ${msg.data.byteLength} ${value}`);
            break;
          }

          // console.log('dev_value item_id:', item_id, 'raw_value:', raw_value, 'value:' + value);

          // console.log(`Parse value ${item_id} ${raw_value} ${value}`);
          const dev_item = this.procDevItemValue(item_id, raw_value, value);
          if (dev_item)
            dev_item_list.push(dev_item);
        }

        if (dev_item_list.length)
          this.dev_item_changed.next(dev_item_list);

        if (idx != msg.data.byteLength) {
          console.warn(`BAD PARSE POSITION ${idx} NEED ${msg.data.byteLength} ${JSON.stringify(view)}`);
        }
      } else if (msg.cmd == WebSockCmd.WS_CHANGE_GROUP_PARAM_VALUES) {
          const find_param = (params: DIG_Param[], prm_id: number): DIG_Param => {
              if (params) {
                  for (let p of params) {
                      if (p.id === prm_id)
                          return p;

                      const param = find_param(p.childs, prm_id);
                      if (param)
                          return param;
                  }
              }

              return null;
          };

        const set_param_impl = (group: Device_Item_Group, prm_id: number, value: string) => {
            const param = find_param(group?.params, prm_id);
            if (param) {
                let parsedValue: any = value;

                if (param.param.value_type === DIG_Param_Value_Type.VT_BOOL && typeof value === 'string') {
                    parsedValue = value !== 'false' && value !== '0';
                }

                param.value = parsedValue;
            }
            return param;
        };

        let last_group: Device_Item_Group;
        const set_param = (prm_id: number, value: string) => {
            let param = set_param_impl(last_group, prm_id, value);
          if (param) {
            return param;
          }
          for (const sct of this.schemeService.scheme.section) {
            for (const group of sct.groups) {
                param = set_param_impl(group, prm_id, value);
              if (param) {
                if (last_group !== group) {
                  last_group = group;
                }
                return param;
              }
            }
          }
        };

        const view = new Uint8Array(msg.data);
        let [idx, count] = ByteTools.parseUInt32(view);
        let param_id: number;
        let user_id: number;
        let ts: number;

        const changed_params: DIG_Param[] = [];
        const logs: Log_Param[] = [];
        while (count--) {
            ts = ByteTools.parseInt64(view, idx)[1];
            ts &= ~0x80000000000000;
            idx += 8;
            user_id = ByteTools.parseUInt32(view, idx)[1];
            idx += 4;
            param_id = ByteTools.parseUInt32(view, idx)[1];
            idx += 4;

            const [last_pos, value] = ByteTools.parseQString(view, idx);
            idx = last_pos;

            const changed_param = set_param(param_id, value);
            const paramLog: Log_Param = {
                value,
                user_id,
                group_param_id: param_id,
                timestamp_msecs: ts,
            };
            logs.push(paramLog);
            changed_params.push(changed_param);
        }

        if (changed_params.length > 0) {
            this.group_param_values_changed.next(changed_params);
        }
        if (logs.length > 0) {
            this.log_param.next(logs);
        }
      } else if (msg.cmd == WebSockCmd.WS_GROUP_STATUS_ADDED) {
        const view = new Uint8Array(msg.data);
        const group_id = ByteTools.parseUInt32(view)[1];
        const type_id = ByteTools.parseUInt32(view, 4)[1];
        let [idx, args_count] = ByteTools.parseUInt32(view, 8);
        const args: string[] = [];

        while (args_count--) {
          const [last_pos, value] = ByteTools.parseQString(view, idx);
          idx = last_pos;
          args.push(value);
        }

        for (const sct of this.schemeService.scheme.section) {
          for (const group of sct.groups) {
            if (group.id == group_id) {
              if (group.statuses === undefined) {
                group.statuses = [];
              }
              for (const gsts of group.statuses) {
                if (gsts.status_id == type_id) {
                  gsts.args = args;
                  return;
                }
              }

              let status_type: DIG_Status_Type;
              for (const sts of this.schemeService.scheme.dig_status_type) {
                if (sts.id == type_id) {
                  status_type = sts;
                }
              }

              if (status_type === undefined) {
                console.warn(`Status id ${type_id} not found`);
              } else {
                group.statuses.push({status: status_type, args: args, status_id: type_id});
                this.schemeService.calculateStatusInfo(group);
              }
              return;
            }
          }
        }

      } else if (msg.cmd == WebSockCmd.WS_GROUP_STATUS_REMOVED) {
        const view = new Uint8Array(msg.data);
        const group_id = ByteTools.parseUInt32(view)[1];
        const info_id = ByteTools.parseUInt32(view, 4)[1];
        for (const sct of this.schemeService.scheme.section) {
          for (const group of sct.groups) {
            if (group.id == group_id) {
              if (group.statuses === undefined) {
                group.statuses = [];
              }

              let l = group.statuses.length;
              while (l--) {
                //console.log(JSON.parse(JSON.stringify(group.statuses[l])));
                if (group.statuses[l].status_id == info_id) {
                  group.statuses.splice(l, 1);
                  this.schemeService.calculateStatusInfo(group);
                  return;
                }
              }

              return;
            }
          }
        }
      } else if (msg.cmd === WebSockCmd.WS_STREAM_DATA
                 || msg.cmd === WebSockCmd.WS_STREAM_TOGGLE
                 || msg.cmd === WebSockCmd.WS_STREAM_TEXT) {
        this.stream_msg.next(msg);
      } else {
        this.byte_msg.next(msg);
      }
    });

    const protocol = window.location.protocol.replace('http', 'ws'); // http: -> ws: , https: -> wss:
    const host = window.location.host; // With port -> localhost:4200
    const ws_url = `${protocol}//${host}/${protocol}`;
    this.wsbService.start(ws_url.replace(/:$/, '/'));
  }

  close(): void {
    this.bmsg_sub.unsubscribe();
    this.wsbService.close();
  }

  private procDevItemValue(item_id: number, raw_value: any, value: any): Device_Item {
    const item: Device_Item = this.schemeService.devItemById(item_id);
    if (item) {
      if (!item.val) {
        item.val = {raw_value, value};
      } else {
        item.val.raw_value = raw_value;
        item.val.value = value;
      }
    }

    return item;
  }

  private static parseLogs<T extends Log_Base>(data: ArrayBuffer, subj: Subject<T[]>, construct: new () => T, parser: (view: Uint8Array, meta: { idx: number; }, item: T) => T) {
    const view = new Uint8Array(data);
    const meta = { idx: undefined };
    const item = new construct();
    let count;
    [meta.idx, count] = ByteTools.parseUInt32(view);

    const log_pack: T[] = [];
    while (count--) {
      item.timestamp_msecs = ByteTools.parseInt64(view, meta.idx)[1];
      item.timestamp_msecs &= ~0x80000000000000;
      meta.idx += 8;
      item.user_id = ByteTools.parseUInt32(view, meta.idx)[1];
      meta.idx += 4;

      log_pack.push(parser(view, meta, item));
    }

    if (log_pack.length)
        subj.next(log_pack);
  }

  private static logModeParser(view: Uint8Array, meta: { idx: number; }, item: Log_Mode): Log_Mode {
    item.group_id = ByteTools.parseUInt32(view, meta.idx)[1];
    meta.idx += 4;
    item.mode_id = ByteTools.parseUInt32(view, meta.idx)[1];
    meta.idx += 4;

    return {...item} as Log_Mode;
  }

  private static logStatusParser(view: Uint8Array, meta: { idx: number; }, item: Log_Status): Log_Status {
    item.direction = view[meta.idx - 4 - 8] & 0x80 ? Log_Status_Direction.SD_DEL : Log_Status_Direction.SD_ADD;
    item.group_id = ByteTools.parseUInt32(view, meta.idx)[1];
    meta.idx += 4;
    item.status_id = ByteTools.parseUInt32(view, meta.idx)[1];
    meta.idx += 4;

    item.args = [];
    let args_count = ByteTools.parseUInt32(view, meta.idx)[1];
    let value: string;
    while (args_count--) {
      [meta.idx, value] = ByteTools.parseQString(view, meta.idx);
      item.args.push(value);
    }

    return {...item} as Log_Status;
  }

  parseConnectNumber(n: number) {
    // tslint:disable:no-bitwise
    const connState = n & ~Connection_State.CS_CONNECTED_MODIFIED & ~Connection_State.CS_CONNECTED_WITH_LOSSES;
    const modState = (n & Connection_State.CS_CONNECTED_MODIFIED) === Connection_State.CS_CONNECTED_MODIFIED;
    const losesState = (n & Connection_State.CS_CONNECTED_WITH_LOSSES) === Connection_State.CS_CONNECTED_WITH_LOSSES;
    // tslint:enable:no-bitwise

    return [connState, modState, losesState];
  }

  parseConnectState(data: ArrayBuffer): any[] {
    if (data === undefined) {
      return;
    }

    const view = new Uint8Array(data);

    return this.parseConnectNumber(view[0]);
  }

  parseTimeInfo(data: ArrayBuffer): Time_Info {
    if (data === undefined) {
      return {utc_time: 0, tz_name: "", tz_offset: 0};
    }

    const view = new Uint8Array(data);

    const [start1, utc_time] = ByteTools.parseInt64(view, 0);
    const [start2, tz_name] = ByteTools.parseQString(view, start1);
    const [start3, tz_offset] = ByteTools.parseInt32(view, start2);
    return {utc_time, tz_name, tz_offset};
  }

  /* DEPRECATED */
  parseConnectInfo(data: ArrayBuffer): ConnectInfo {
    if (data === undefined) {
      return;
    }

    const view = new Uint8Array(data);
    const connected: boolean = view[0] == 1;
    const [start, ip] = ByteTools.parseQString(view, 1);
    const [start1, time] = ByteTools.parseInt64(view, start);
    const [start2, time_zone] = ByteTools.parseQString(view, start1);
    const modified: boolean = view[start2] == 1;
    return {connected, ip, time, time_zone, modified};
  }

  parseEventMessage(data: ArrayBuffer): Log_Event[] {
    if (data === undefined) {
      return;
    }


    const items: Log_Event[] = [];
    const view = new Uint8Array(data);
    let [start1, count] = ByteTools.parseUInt32(view);
    while (count--) {
      const [start2, time_ms] = ByteTools.parseInt64(view, start1);
      const [start3, user_id] = ByteTools.parseUInt32(view, start2);
      const type = view[start3] & ~0x80;
      const [start5, who] = ByteTools.parseQString(view, start3 + 1);
      const [start6, msg] = ByteTools.parseQString(view, start5);
      start1 = start6;
      items.push({timestamp_msecs: time_ms, text: msg, category: who, type_id: type, user_id, color: ''} as Log_Event);
    }
    return items;
  }

  getConnectInfo(): void {
    this.wsbService.send(WebSockCmd.WS_CONNECTION_STATE, this.schemeService.scheme.id);
  }

  writeToDevItem(item_id: number, value: any): void {
    const value_buf = ByteTools.saveQVariant(value);

    const view = new Uint8Array(4 + value_buf.length);
    ByteTools.saveInt32(item_id, view);
    view.set(value_buf, 4);

    this.wsbService.send(WebSockCmd.WS_WRITE_TO_DEV_ITEM, this.schemeService.scheme.id, view);
  }

  changeGroupMode(value: any, group_id: number): void {
    const view = new Uint8Array(8);
    ByteTools.saveInt32(+value, view);
    ByteTools.saveInt32(group_id, view, 4);
    console.log(`MODE ${value} GROUP ${group_id}`);
    this.wsbService.send(WebSockCmd.WS_CHANGE_GROUP_MODE, this.schemeService.scheme.id, view);
  }

  changeParamValues(params: DIG_Param[]): void {
    if (!params.length) {
      return;
    }

    let msg_size = 0;
    let string_buf: Uint8Array;
    let view: Uint8Array;
    const data_list: Uint8Array[] = [];
    for (const data of params) {
      string_buf = ByteTools.saveQString(data.value ? data.value.toString() : null);
      view = new Uint8Array(4 + string_buf.length);
      ByteTools.saveInt32(data.id, view);
      view.set(string_buf, 4);
      data_list.push(view);
      msg_size += view.length;
    }

    view = new Uint8Array(4 + msg_size);
    ByteTools.saveInt32(params.length, view);
    let start = 4;
    for (const data of data_list) {
      view.set(data, start);
      start += data.length;
    }

    this.wsbService.send(WebSockCmd.WS_CHANGE_GROUP_PARAM_VALUES, this.schemeService.scheme.id, view);
  }

  restart(): void {
    this.wsbService.send(WebSockCmd.WS_RESTART, this.schemeService.scheme.id);
  }

  execScript(script: string) {
    const view = ByteTools.saveQString(script);
    this.wsbService.send(WebSockCmd.WS_EXEC_SCRIPT, this.schemeService.scheme.id, view);
  }

  exec_function(func_name: string, args: any[]): void {
    const arg_list: any[] = [];
    let args_size = 0;
    for (const arg of args) {
      const arg_var = ByteTools.saveQVariant(arg);
      args_size += arg_var.length;
      arg_list.push(arg_var);
    }

    const func_name_view = ByteTools.saveQString(func_name);
    const view = new Uint8Array(func_name_view.length + 4 + args_size);
    let pos = 0;
    view.set(func_name_view, pos);
    pos += func_name_view.length;
    ByteTools.saveInt32(args.length, view, pos);
    pos += 4;
    for (const data of arg_list) {
      view.set(data, pos);
      pos += data.length;
    }
    this.wsbService.send(WebSockCmd.WS_EXEC_SCRIPT, this.schemeService.scheme.id, view);
  }

    stream_toggle(dev_item_id: number, state: boolean): void
    {
        let view = new Uint8Array(4 + 1);
        let pos = 0;

        ByteTools.saveInt32(dev_item_id, view, pos); pos += 4;
        view[pos] = state ? 1 : 0; pos += 1;

        this.wsbService.send(WebSockCmd.WS_STREAM_TOGGLE, this.schemeService.scheme.id, view);
    }
} // end class ControlService
