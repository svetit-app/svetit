import { Component, ViewChild, ElementRef, OnInit, Inject, OnDestroy } from '@angular/core';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { SubscriptionLike } from "rxjs";

import { ByteMessage, ByteTools } from '../../../web-socket.service';
import { ControlService, WebSockCmd } from "../../control.service";
import { Device_Item, Log_Value } from '../../scheme';

export interface VideoStreamParam {
    isImg: boolean;
    devItem: Device_Item;
    img: Log_Value;
}

@Component({
  selector: 'app-video-stream-dialog',
  templateUrl: './video-stream-dialog.component.html',
  styleUrls: ['./video-stream-dialog.component.css']
})
export class VideoStreamDialogComponent implements OnInit, OnDestroy {
    @ViewChild('image', { static: true })
    img: ElementRef;

    name: string;

    width: number = 800;
    height: number = 600;
    image_data: ImageData;

    timer_id: any;

    need_adjust: boolean = true;

    sub: SubscriptionLike;

    constructor(
      private controlService: ControlService,
      public dialogRef: MatDialogRef<VideoStreamDialogComponent>,
      @Inject(MAT_DIALOG_DATA) public data: VideoStreamParam)
    {
    }

    ngOnInit(): void {
        this.width  = window.innerWidth - 50;
        this.height = window.innerHeight - 170;

        this.img.nativeElement.onload = () => {
            const img = this.img.nativeElement;

            const max_width = window.innerWidth - 50;
            const max_height = window.innerHeight - 170;
            const w_pt = img.naturalWidth / 100.0;
            const h_pt = img.naturalHeight / 100.0;
            const pt = Math.min(max_width / w_pt, max_height / h_pt);
            this.width = Math.floor(w_pt * pt);
            this.height = Math.floor(h_pt * pt);

            if (this.need_adjust)
                this.need_adjust = false;
            else
                img.onload = null;
        };

        if (this.data.isImg)
            this.fillImg();
        else
            this.initVideo();
    }

    static get_default_settings(): any
    {
        return {
            autoFocus: false,
            maxWidth: '100%',
            width: '100%',
            maxHeight: '100%',
            height: '100%',
            panelClass: 'imgDialog',
        };
    }

    fillImg(): void {
        const obj = (<any>this.data.img)
        this.name = (obj.item.name || obj.item.type.title) + ' ' + obj.date.toString();

        const jpeg_str = atob(this.data.img.raw_value.slice(4)); // 4 is "img:AAAAaa..."
        const jpeg_data = new Uint8Array(jpeg_str.length);
        for (let i = 0; i < jpeg_str.length; ++i)
            jpeg_data[i] = jpeg_str[i].charCodeAt(0);
        this.set_data(jpeg_data);
    }

    initVideo(): void
    {
        this.fill_image('black', 'Подождите...', 'white');

        this.name = this.data.devItem.name || this.data.devItem.type.name;

        this.sub = this.controlService.stream_msg.subscribe((msg: ByteMessage) => {
            if (msg.cmd == WebSockCmd.WS_STREAM_DATA) {
                this.draw_frame(msg.data);
            } else if (msg.cmd === WebSockCmd.WS_STREAM_TEXT) {
                this.draw_text(msg.data);
            } else /*if (msg.cmd === WebSockCmd.WS_STREAM_TOGGLE)*/ {
                this.stream_toggled(msg.data);
            }
        });

        this.timer_id = setTimeout(() => this.stream_start_timeout(), 3000);
        this.controlService.stream_toggle(this.data.devItem.id, true);
    }

    fill_image(color: string, text: string, text_color: string): void
    {
        let canvas = document.createElement('canvas');
        canvas.width = this.width;
        canvas.height = this.height;
        let ctx = canvas.getContext('2d');
        ctx.fillStyle = color;
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        ctx.font = "48px serif";
        const m_text = ctx.measureText(text);
        const x = canvas.width / 2 - (m_text.width / 2);
        const y = canvas.height / 2 - 5;
        ctx.fillStyle = text_color;
        ctx.fillText(text, x, y);
        this.img.nativeElement.src = canvas.toDataURL();
    }

    ngOnDestroy(): void
    {
        if (!this.data.isImg) {
            this.controlService.stream_toggle(this.data.devItem.id, false);
            this.sub.unsubscribe();
        }
    }

    onNoClick(): void {
      this.dialogRef.close();
    }

    draw_text(data: ArrayBuffer): void
    {
        let view = new Uint8Array(data);
        const text = ByteTools.parseQString(view)[1];
        this.fill_image('black', text, 'white');
    }

    draw_frame(data: ArrayBuffer): void
    {
        let view = new Uint8Array(data);
        let pos = 0;
        const dev_item_id = ByteTools.parseUInt32(view, pos)[1]; pos += 4;
        if (this.data.devItem.id !== dev_item_id)
        {
            console.warn("Unknown stream device", dev_item_id, this.data.devItem.id);
            return;
        }

        const jpeg_data = view.slice(4); // QByteArray size skip
        this.set_data(jpeg_data);
    }

    set_data(jpeg_data: Uint8Array): void
    {
        const blob = new Blob( [ jpeg_data ], { type: "image/jpeg" } );
        const url_creator = window.URL || window.webkitURL;
        const image_url = url_creator.createObjectURL( blob );
        this.img.nativeElement.src = image_url;
    }

    stream_toggled(data: ArrayBuffer): void
    {
        let view = new Uint8Array(data);
        let pos = 0;
        const user_id = ByteTools.parseUInt32(view, pos)[1]; pos += 4;
        const dev_item_id = ByteTools.parseUInt32(view, pos)[1]; pos += 4;
        const state = view[pos] == 1; pos += 1;

        console.log("Stream toggled:", dev_item_id, state);
        if (this.data.devItem.id !== dev_item_id)
            return;

        if (state)
            clearTimeout(this.timer_id);
        else
            this.dialogRef.close();
    }

    stream_start_timeout(): void
    {
        this.fill_image('red', 'Ошибка', 'black');
    }
}
