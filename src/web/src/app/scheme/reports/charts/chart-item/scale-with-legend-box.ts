import {ChartArea, LinearScale, Scale} from 'chart.js';

export class ScaleWithLegendBox extends LinearScale implements Scale {
    public static id = 'LinearWithLegend';

    draw(chartArea: ChartArea) {
        super.draw(chartArea);
        // @ts-ignore
        if (!this._isVisible()) {
            return;
        }

        this.drawLegend();
    }

    drawLegend() {
        const legend = this.getLegend();

        const dsCount = legend.length;
        const s = Math.sqrt(dsCount);
        const rowCount = Math.floor(s);
        let columnCount = Math.ceil(s);

        if (rowCount === 1) {
            columnCount = dsCount;
        }

        const maxWidth = this.right - this.left;
        const width = maxWidth > 30 ? 30 : maxWidth;
        const height = 30;
        const ctx = this.chart.canvas.getContext('2d');

        const field_width = width / columnCount;
        const field_height = height / rowCount;

        const padding = (this.right - this.left - width) / 2;
        const boxX = this.left + padding;
        const boxY = this.bottom + 10;

        for (let i = 0; i < legend.length; ++i) {
            ctx.fillStyle = legend[i];
            const row = Math.floor(i / columnCount);
            const column = i % columnCount;
            const x = column * field_width + boxX;
            const y = row * field_height + boxY;

            ctx.fillRect(x, y, field_width, field_height);
        }
    }

    private getLegend() {
        return this.chart.config.data.datasets
            // @ts-ignore
            .filter((ds) => ds.yAxisID === this.id)
            .map((ds) => ds.borderColor as string);
    }
}
