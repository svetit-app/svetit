import { Component, ViewChild, ElementRef, OnInit, OnDestroy } from '@angular/core';

@Component({
  selector: 'app-drag-scroll',
  template: `
    <div #container
      class="schemetitle"
      (mousemove)="move($event)"
      (mousedown)="start($event)"
      (mouseup)="end($event)"
      (touchend)="end($event)"
      (touchcancel)="end($event)"
      (touchstart)="onTouchStart($event)"
      (touchmove)="onTouchMove($event)"
    ><span><ng-content></ng-content></span></div>`,
  styles: [`
    :host {
      display: grid;
      margin: 0 8px;
    }
    .schemetitle {
        height: 100%;
        overflow: hidden;

        cursor: ew-resize;

        white-space: nowrap;
        line-height: 32px;
        font-size: 21px;
        font-weight: bold;
        font-family: Roboto, "Helvetica Neue", sans-serif;
        color: #999;

        -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
        -khtml-user-select: none; /* Konqueror HTML */
        -moz-user-select: none; /* Firefox */
        -ms-user-select: none; /* Internet Explorer/Edge */
        user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }
    @media all and (max-width: 600px) {
        .schemetitle {
            font-size: 18px;
        }
    }
    @keyframes scroll {
          0% { transform: translate(-15%, 0); }
         50% { transform: translate(-85%, 0); }
        100% { transform: translate(-15%, 0); }
    }
    .scroll-active {
        display: inline-block;
        padding-left: 100%;
        animation: scroll 35s infinite linear;
    }
    `]
})
export class DragScrollComponent implements OnInit, OnDestroy {

  @ViewChild('container', {static: true}) elem: ElementRef;

  isPressed: boolean = false;
  downX: number = 0;
    direction: number = -1;

    _timeout: any = null;
    _interval: any = null;

    ngOnInit() {
        this.startTimer();
    }

    ngOnDestroy() {
        this.stopTimer();
    }

    startTimer() {
        if (!this._interval) {
            if (this._timeout) {
                this.stopTimer();

                const el = this.elem.nativeElement;
                if (el.clientWidth !== el.scrollWidth)
                {
                    // el.children[0].classList.add("scroll-active"); // Профайлер вроде как показал что бегущая строка на CSS тратит больше ресурсов
                    this._interval = setInterval(() => this.autoMove(), 50);
                }
            } else
                this._timeout = setTimeout(() => this.startTimer(), 3000);
        }
    }

    stopTimer() {
        if (this._timeout) {
            clearTimeout(this._timeout);
            this._timeout = null;
        }
        if (this._interval) {
            clearInterval(this._interval);
            this._interval = null;
        }
        this.elem.nativeElement.children[0].classList.remove("scroll-active");
    }

    autoMove() {
        const el = this.elem.nativeElement;
        if (el.scrollLeft === el.scrollWidth - el.clientWidth)
            this.direction = 1;
        else if (el.scrollLeft === 0)
            this.direction = -1;

        const clientX = this.downX + this.direction;
        el.scrollLeft = el.scrollLeft - clientX + this.downX;
        this.downX = clientX;
    }

  start(event: any): void {
      this.stopTimer();
    this.isPressed = true;
    this.downX = event.clientX;
  }

  end(event: any): void {
    this.isPressed = false;
      this.startTimer();
  }

  move(event: any) {
    if (!this.isPressed)
      return;

    const el = this.elem.nativeElement;
    el.scrollLeft = el.scrollLeft - event.clientX + this.downX;
    this.downX = event.clientX;
  }

  onTouchStart(event: any): void {
    this.start(event.changedTouches[0]);
  }

  onTouchMove(event: any): void {
    this.move(event.changedTouches[0]);
  }
}
