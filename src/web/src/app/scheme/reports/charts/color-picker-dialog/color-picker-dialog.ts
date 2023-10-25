import { Component, OnInit, Inject, ViewChild, ElementRef } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import {Chart_Params} from '../chart-types';
import {Chart_Item} from '../../../scheme';

export interface Hsl {
    h: number;
    s: number;
    l: number;
}

export interface Rgb {
    r: number;
    g: number;
    b: number;
}

export interface DialogData {
  dataset: Chart_Item<any>;
}

@Component({
    selector: 'color-picker-dialog',
    templateUrl: 'color-picker-dialog.html',
    styleUrls: ['color-picker-dialog.css']
})
export class ColorPickerDialog implements OnInit {
    @ViewChild('hueLine', { static: true }) canvasElem: ElementRef;
    canvas: any;
    ctx: any;

    cur: number = null; // X pos
    curColor: string = 'white';
    color: string;

    get hsl(): Hsl
    {
        if (this.cur)
        {
            const p = this.ctx.getImageData(this.cur, this.canvas.height / 2, 1, 1).data;
            return ColorPickerDialog.rgb2hsl(p[0], p[1], p[2]);
        }
        return null;
    }

    constructor(
      public dialogRef: MatDialogRef<ColorPickerDialog>,
      @Inject(MAT_DIALOG_DATA) public data: DialogData) {}

    ngOnInit(): void {
        this.canvas = this.canvasElem.nativeElement;
        this.ctx = this.canvas.getContext('2d');
        this.draw();

        this.color = this.data.dataset.extra.displayColor;
        this.cur = this.findColor(this.color);
        if (this.cur)
        {
            this.curColor = this.color;
            this.draw();
        }
    }

    findColor(hslStr: string): number
    {
        const hsl = ColorPickerDialog.parseHsl(hslStr);
        if (!hsl)
            return null;

        const y = this.canvas.height / 2;
        for (let x = 0; x < this.canvas.width; ++x)
        {
            const p = this.ctx.getImageData(x, y, 1, 1).data;
            const pHsl = ColorPickerDialog.rgb2hsl(p[0], p[1], p[2]);
            if (pHsl.h == hsl.h)
                return x;
        }

        return null;
    }

    draw() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

        this.ctx.lineWidth = 1;
        this.ctx.strokeStyle = "#000";
        this.ctx.strokeRect(0.5, 2.5, this.canvas.width - 1, this.canvas.height - 5);

        let gradient = this.ctx.createLinearGradient(0, 0, this.canvas.width, 0);
        gradient.addColorStop(0, 'hsl(0, 100%, 30%)');
        gradient.addColorStop(0.16, 'hsl(60, 100%, 30%');
        gradient.addColorStop(0.32, 'hsl(120, 100%, 30%)');
        gradient.addColorStop(0.48, 'hsl(180, 100%, 30%)');
        gradient.addColorStop(0.64, 'hsl(240, 100%, 30%)');
        gradient.addColorStop(0.8, 'hsl(300, 100%, 30%)');
        gradient.addColorStop(1, 'hsl(360, 100%, 30%)');

        this.ctx.fillStyle = gradient;
        this.ctx.fillRect(2, 3, this.canvas.width - 4, this.canvas.height - 6);

        if (this.cur != null)
        {
            this.ctx.lineWidth = 2;
            this.ctx.strokeStyle = "#000";
            this.ctx.strokeRect(this.cur - 4.5, 0.5, 8, this.canvas.height - 1);
        }
    }

    getMouseX(e)
    {
        return Math.round((e.offsetX / (this.canvas.getBoundingClientRect().width / 100)) * (this.canvas.width / 100));
    }

    getColor(x)
    {
        const p = this.ctx.getImageData(x, this.canvas.height / 2, 1, 1).data;
        return `rgb(${p[0]}, ${p[1]}, ${p[2]})`;
    }

    onMouseMove(e) {
        this.curColor = this.getColor(this.getMouseX(e));
    }

    onMouseOut()
    {
        if (this.cur)
            this.curColor = this.color;
    }

    onClick(e)
    {
        this.cur = this.getMouseX(e);
        this.color = this.getColor(this.cur);
        this.onMouseOut();
        this.draw();
    }

    onNoClick(): void {
        this.dialogRef.close();
    }

    static parseHsl(hslStr: string): any
    {
        const match = hslStr.match(/hsla?\((\d{1,3}), ?(\d{1,3})%?, ?(\d{1,3})%?\)?(?:, ?(\d(?:\.\d?))\))?/);
        if (!match)
            return null;

        const h = parseInt(match[1]);
        const s = parseInt(match[2]);
        const l = parseInt(match[3]);
        if (isNaN(h) || isNaN(s) || isNaN(l))
            return null;

        return { h, s, l };
    }

    static hslStr2RgbHex(hslStr: string): string
    {
        const hsl = ColorPickerDialog.parseHsl(hslStr);
        if (!hsl)
            return null;

        return this.hsl2RgbStr(hsl);
		// return "rgb("+ (+r + "," + +g + "," + +b) + ")";
    }

    static hsl2RgbStr(hsl: Hsl): string {
        const h = hsl.h, s = hsl.s / 100, l = hsl.l / 100;

        let c = (1 - Math.abs(2 * l - 1)) * s,
            x = c * (1 - Math.abs((h / 60) % 2 - 1)),
            m = l - c/2,
            r = 0, g = 0, b = 0;

        if (0 <= h && h < 60) {
            r = c; g = x; b = 0;
        } else if (60 <= h && h < 120) {
            r = x; g = c; b = 0;
        } else if (120 <= h && h < 180) {
            r = 0; g = c; b = x;
        } else if (180 <= h && h < 240) {
            r = 0; g = x; b = c;
        } else if (240 <= h && h < 300) {
            r = x; g = 0; b = c;
        } else if (300 <= h && h < 360) {
            r = c; g = 0; b = x;
        }

        r = Math.round((r + m) * 255);
        g = Math.round((g + m) * 255);
        b = Math.round((b + m) * 255);

        let cToHex = num => {
            const hex = num.toString(16);
            return hex.length == 1 ? "0" + hex : hex;
        };

        return `#${cToHex(r)}${cToHex(g)}${cToHex(b)}`;
    }

    static rgbhex2hsl(rgbHex: string): Hsl
    {
        if (!rgbHex)
            return null;

        const r = parseInt(rgbHex.substr(1,2), 16);
        const g = parseInt(rgbHex.substr(3,2), 16);
        const b = parseInt(rgbHex.substr(5,2), 16);
        if (isNaN(r) || isNaN(g) || isNaN(b))
            return null;

        return ColorPickerDialog.rgb2hsl(r, g, b);
		// return "hsl(" + h + "," + s + "%," + l + "%)";
    }

    static rgb2hsl(r: number, g: number, b: number): Hsl
    {
        // Make r, g, and b fractions of 1
        r /= 255;
        g /= 255;
        b /= 255;

        // find greatest and smallest channel values
        let cmin = Math.min(r,g,b),
			cmax = Math.max(r,g,b),
			delta = cmax - cmin,
			h = 0, s = 0, l = 0;

		// calculate hue
		// no difference
		if (delta == 0)
			h = 0;
		// red is max
		else if (cmax == r)
			h = ((g - b) / delta) % 6;
		// green is max
		else if (cmax == g)
			h = (b - r) / delta + 2;
		// blue is max
		else
			h = (r - g) / delta + 4;

		h = Math.round(h * 60);

		// make negative hues positive behind 360°
		if (h < 0)
			h += 360;

		// calculate lightness
		l = (cmax + cmin) / 2;

		// calculate saturation
		s = delta == 0 ? 0 : delta / (1 - Math.abs(2 * l - 1));

		// multiply l and s by 100
		s = +(s * 100).toFixed(1);
		l = +(l * 100).toFixed(1);

        return { h, s, l } as Hsl;
    }
}

