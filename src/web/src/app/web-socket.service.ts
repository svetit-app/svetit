import { Injectable } from '@angular/core';

import { Subject, Observable, Subscription, Observer, Subscriber }		from 'rxjs';
import { map } from 'rxjs/operators';
import { webSocket, WebSocketSubject, WebSocketSubjectConfig } from 'rxjs/webSocket';

import { AuthService } from './auth/service';

enum WebSockCmd {
	WS_UNKNOWN,
	WS_AUTH,
	WS_WELCOME,

	WEB_SOCK_CMD_COUNT
}

export interface ByteMessage {
	cmd: number;
	scheme_id: number;
	data: ArrayBuffer;
}

export class ByteTools {
	static extendBuffer(view: Uint8Array, extend_size: number): Uint8Array {
		const new_view = new Uint8Array(view.length + extend_size);
		new_view.set(view);
		return new_view;
	}

	static saveInt32(value: number, out: Uint8Array, start: number = 0): void {
		out[start] = (value & 0xff000000) >> 24;
		out[start + 1] = (value & 0x00ff0000) >> 16;
		out[start + 2] = (value & 0x0000ff00) >> 8;
		out[start + 3] = (value & 0x000000ff);
	}

	static saveInt16(value: number, out: Uint8Array, start: number = 0): void {
		out[start] = (value & 0x0000ff00) >> 8;
		out[start + 1] = (value & 0x000000ff);
	}

	static saveByteArray(data_arr: string): Uint8Array {
		const t_len = data_arr.length;
		const data = new Uint8Array(t_len + 4);

		ByteTools.saveInt32(t_len, data);
		for (let i = 0; i < t_len; ++i) {
			data[4 + i] = data_arr.charCodeAt(i);
		}
		return data;
	}

	static saveQString(value: string, is_null: boolean = undefined): Uint8Array {
		if (is_null === undefined) {
			is_null = !value;
		}
		const length = (is_null || !value) ? 0 : value.length;
		const view = new Uint8Array(4 + (length * 2));

		if (is_null) {
			ByteTools.saveInt32(0xFFFFFFFF, view);
		} else {
			ByteTools.saveInt32(length * 2, view);

			let code: number;
			for (let i = 0; i < length; ++i) {
				ByteTools.saveInt16(value.charCodeAt(i), view, 4 + (i * 2));
			}
		}

		return view;
	}

	static saveDouble(value: number): Uint8Array {
		const buf = new ArrayBuffer(8);
		const view = new Uint8Array(buf);
		const dbl_view = new Float64Array(buf);
		dbl_view[0] = value;

		let val: number;
		for (let i = 0; i < 4; ++i) {
			val = view[i];
			view[i] = view[7 - i];
			view[7 - i] = val;
		}
		return view;
	}

	static saveQVariant(value: any, is_null: boolean = false): Uint8Array {
		let variant_type: number;
		let data: Uint8Array;

		if (!is_null) {
			is_null = value === undefined || value === null;
		}

		if (Array.isArray(value)) {
			variant_type = 9; // 9 QVariantList
			let total_size = 0;
			const items = [];
			for (const item of value) {
				const val = ByteTools.saveQVariant(item);
				total_size += val.length;
				items.push(val);
			}

			data = new Uint8Array(4 + total_size);
			let pos = 0;
			ByteTools.saveInt32(items.length, data, pos); pos += 4;
			for (const item of items) {
				data.set(item, pos); pos += item.length;
			}
		} else {
			const t_str = typeof value;
			if (t_str === 'string') {
				variant_type = 10; // 10 QString
				data = ByteTools.saveQString(value);
			} else if (t_str === 'number') {
				if (value % 1 === 0) {
					variant_type = 2; // 2 int
					data = new Uint8Array(4);
					ByteTools.saveInt32(value, data);
				} else {
					variant_type = 6; // 6 double
					data = ByteTools.saveDouble(value);
				}
			} else if (t_str === 'boolean') {
				variant_type = 1; // 1 bool
				data = new Uint8Array(1);
				data[0] = value ? 1 : 0;
			} else if (t_str === 'object') {
				console.log(value);
				variant_type = 8; // 8 QVariantMap
				data = ByteTools.saveQVariantMap(value);
			} else {
				variant_type = 0; // 0 Invalid
				is_null = true;
			}
		}

		const view = new Uint8Array(5 + (data ? data.length : 0));
		ByteTools.saveInt32(variant_type, view);
		view[4] = is_null ? 1 : 0;
		if (data) {
			view.set(data, 5);
		}
		return view;
	}

	static saveQVariantList(obj: any): Uint8Array {
		if (typeof obj === 'string') {
			obj = JSON.parse(obj);
		}
		let total_size = 0;
		const items = [];
		for (const value of obj) {
			const value_view = ByteTools.saveQVariant(value);
			total_size += value_view.length;
			items.push(value_view);
		}

		const view = new Uint8Array(4 + total_size);
		let pos = 0;
		ByteTools.saveInt32(items.length, view, pos); pos += 4;

		for (const item of items) {
			view.set(item, pos); pos += item.length;
		}

		return view;
 }

	static saveQVariantMap(obj: any): Uint8Array {
		if (typeof obj === 'string') {
			obj = JSON.parse(obj);
		}
		let total_size = 0;
		const items = [];
		for (const key in obj) {
			const key_view = ByteTools.saveQString(key);
			const value_view = ByteTools.saveQVariant(obj[key]);
			total_size += key_view.length + value_view.length;
			items.push([key_view, value_view]);
		}

		const view = new Uint8Array(4 + total_size);
		let pos = 0;
		ByteTools.saveInt32(items.length, view, pos); pos += 4;

		for (const item of items) {
			view.set(item[0], pos); pos += item[0].length;
			view.set(item[1], pos); pos += item[1].length;
		}

		return view;
	}


	static parseQByteArray(view: Uint8Array, init_start: number = 0): [number, string] {
		const [start, bytes] = ByteTools.parseUInt32(view, init_start);

		if (bytes == 0xffffffff) {
			return [start, null];
		}

		const raw_str = view.slice(start, bytes - start);
		const value: string = String.fromCharCode.apply(null, raw_str);
		return [start + bytes, value];
	}

	static parseQString(view: Uint8Array, init_start: number = 0): [number, string] {
		const [start, bytes] = ByteTools.parseUInt32(view, init_start);

		if (bytes == 0xffffffff) {
			return [start, null];
		}

		if (bytes <= 0) {
			return [start, ''];
		}

		if (bytes & 0x1) {
			console.warn('QString parse error: Corrupt data');
			return [start, undefined];
		}

		const buff = new ArrayBuffer(bytes);
		const view16 = new Uint16Array(buff);
		for (let i = 0; i < bytes; i += 2) {
				view16[i / 2] = view[start + i] << 8 | view[start + i + 1];
	}
		const value: string = String.fromCharCode.apply(null, view16);

		return [start + bytes, value];
	}

	static parseInt32(view: Uint8Array, start: number = 0): [number, number] {
		const value: number = view[start] << 24 | view[start + 1] << 16 | view[start + 2] << 8 | view[start + 3];
		return [start + 4, value];
	}

	static parseUInt32(view: Uint8Array, start: number = 0): [number, number] {
		const [start1, arr] = ByteTools.parseUInt32Array(view, start, 1);
		return [start1, arr[0]];
	}

	static parseUInt32Array(view: Uint8Array, start: number = 0, count: number = 1): [number, Uint32Array] {
		const buffer_size = count * 4;
		if (count <= 0 || (view.length - start) < buffer_size) {
			return [start, new Uint32Array(0)];
		}

		const view32 = new Uint32Array(count);
		for (let i = 0; i < buffer_size; i += 4) {
			view32[i / 4] = view[start + i] << 24 | view[start + i + 1] << 16 | view[start + i + 2] << 8 | view[start + i + 3];
		}
		return [start + buffer_size, view32];
	}

	static parseInt64(view: Uint8Array, start: number = 0): [number, number] {
		const [start1, values] = ByteTools.parseUInt32Array(view, start, 2);
		return [start1, (values[0] & 0x001FFFFF) * 4294967296 + values[1]];
	}

	static parseFloatImpl(view: Uint8Array, start: number = 0, size: number = 4): [number, number] {
		const buff = new ArrayBuffer(size);
		const view1 = new Uint8Array(buff);
		for (let i = 0; i < size; ++i) {
			view1[i] = view[start + ((size - 1) - i)];
		}

		const value = (size === 4 ? new Float32Array(buff) : new Float64Array(buff))[0];
		return [start + size, value];
	}

	static parseFloat(view: Uint8Array, start: number = 0): [number, number] {
		return ByteTools.parseFloatImpl(view, start, 4);
	}

	static parseDouble(view: Uint8Array, start: number = 0): [number, number] {
		return ByteTools.parseFloatImpl(view, start, 8);
	}

	static parseQVariant(view: Uint8Array, start: number = 0): [number, any] {
		const [start1, type] = ByteTools.parseUInt32(view, start);
		const is_null: boolean = view[start1] == 1;
		start = start1 + 1;

		if (type == 1024) {
			const [start2, name] = ByteTools.parseQString(view, start);
			console.error(`QVariant data is UserType ${name}`);
			return [start2, undefined];
		}

		switch (type) {
			case 0:
				return [start, is_null ? null : undefined];
			case 1024: // UserType
				return [start, null];
			case 51: // Nullptr
				return [start + 4, null];
			case 1: // bool
				return [start + 1, view[start] == 1];

			case 34: // Char
			case 40: // SChar
			case 37: // UChar
				return [start + 1, view[start]];

			case 33: // Short
			case 36: // UShort
				return [start + 2, view[start] << 8 | view[start + 1]];

			case 2: // Int
				return ByteTools.parseInt32(view, start);
			case 3: // UInt
				return ByteTools.parseUInt32(view, start);

			case 4: // Longlong
			case 5: // ULonglong
				return ByteTools.parseInt64(view, start);

			case 38: // Float
				return ByteTools.parseFloat(view, start);

			case 6: // Double
				return ByteTools.parseDouble(view, start);

			case 8: // QVariantMap
				let [start1, size] = ByteTools.parseUInt32(view, start);
				let value_obj = {};

				while (size--)
				{
						const [end_key, key] = ByteTools.parseQString(view, start1);
						const [end_value, value] = ByteTools.parseQVariant(view, end_key);
						start1 = end_value;
						value_obj[key] = value;
				}

						console.log('variantmap', value_obj);
				return [start1, value_obj];

			case 9: { // QVariantList
				let [start1, size] = ByteTools.parseUInt32(view, start);
				const values: any[] = [];
				while (size--) {
					const [start_item, value] = ByteTools.parseQVariant(view, start1);
					start1 = start_item;
					values.push(value);
				}
				return [start1, values];
			}

			case 10: // QString
				return ByteTools.parseQString(view, start);

			case 12: // QByteArray
				return ByteTools.parseQByteArray(view, start);

			case 7: // QChar
			case 11: // QStringList
			case 13: // QBitArray
			case 14: // QDate
			case 15: // QTime
			case 16: // QDateTime
			case 17: // QUrl
			case 27: // QRegExp
			case 28: // QVariantHash
			case 30: // QUuid
			case 44: // QRegularExpression
			case 49: // QByteArrayList

			default:
				console.warn(`Parse QVariant unknown type ${type}`);
				return [start, undefined];
		}
	}
}

@Injectable()
export class WebSocketBytesService {
	private ws: WebSocketSubject<ArrayBuffer>;
	//	private config: WebSocketSubjectConfig;
	private socket: Subscription;

	public opened: Subject<boolean> = new Subject<boolean>();
	public message: Subject<ByteMessage> = new Subject<ByteMessage>();

	constructor(
		private auth: AuthService,
	) {}

	public close(): void {
		if (this.socket) {
			this.socket.unsubscribe();
		}
		if (this.ws) {
			this.ws.complete();
		}
	}

	public start(url: string): void {
		const webSockConf: WebSocketSubjectConfig<any> = {
			url: url,
			binaryType: 'arraybuffer',
			resultSelector: (e: MessageEvent) => e.data,
			openObserver: Subscriber.create((e: Event) => this.sendAuth()),
			closeObserver: Subscriber.create((e: CloseEvent) => {
				console.log('ByteWebSocket close');
			}),
			deserializer: function (e) { return e.data; },
			serializer: function (value) { return value; },
		};
		this.ws = webSocket(webSockConf);

		this.socket = this.ws.subscribe({
			next: (msg: ArrayBuffer) => {
				if (!msg.byteLength) {
					return;
				}

				if ((<any>msg).length !== undefined) {
					return;
				}

				const info = new Uint8Array(msg);
				const cmd = info[0];
				const scheme_id = info[1] << 24 | info[2] << 16 | info[3] << 8 | info[4];
				const data = msg.slice(5);

				if (cmd == WebSockCmd.WS_AUTH) {
					this.sendAuth();
				} else {
					if (cmd == WebSockCmd.WS_WELCOME) {
						this.opened.next(true);
					}

					this.message.next({ cmd, scheme_id, data });
				}
			},
			error: (err_text: any) => {
				console.log('WebSocketBytest error'); console.log(err_text);
				this.opened.next(false);

				this.socket.unsubscribe();
				setTimeout(() => this.start(url), 1000);
			},
			complete: () => {
				this.opened.next(false);
				// this.message.next({ cmd: 'closed', scheme_id: undefined, data: undefined });
			}
		});
	}

	public send(cmd: number, scheme_id: number, data: Uint8Array = undefined): void {
		const byte_length = data !== undefined ? data.length : 0;
		const msg = new ArrayBuffer(5 + byte_length);
		const view = new Uint8Array(msg);

		view[0] = cmd;
		ByteTools.saveInt32(scheme_id, view, 1);
		if (data !== undefined) {
			view.set(data, 5);
		}

		this.ws.next(msg);
	}

	private sendAuth(): void {
		const token = this.auth.tokens;

		if (token) {
			this.send(WebSockCmd.WS_AUTH, 0, ByteTools.saveByteArray(token.access));
		} else {
			this.close();
		}

		// const s = "hello"; // If given to the writer, it will be coerced to QString
		// const qbytearray = QByteArray.from(s); // This will write the same string but as a QByteArray
		// this.messages.next(qbytearray);
	}
}
