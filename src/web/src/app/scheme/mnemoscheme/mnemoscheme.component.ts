import {ChangeDetectorRef, Component, ElementRef, OnInit, ViewChild} from '@angular/core';
import {SchemeService} from '../scheme.service';
import {Device_Item_Group, Mnemoscheme} from '../scheme';
import {ControlService} from '../control.service';
import {Subscription} from 'rxjs';
import { NgIf, NgFor } from '@angular/common';
import { MatButton } from '@angular/material/button';
import { DeviceItemGroupComponent } from '../device-item-group/device-item-group.component';

@Component({
    selector: 'app-mnemoscheme',
    templateUrl: './mnemoscheme.component.html',
    styleUrls: ['./mnemoscheme.component.css'],
    standalone: true,
    imports: [NgIf, NgFor, MatButton, DeviceItemGroupComponent]
})
export class MnemoschemeComponent implements OnInit {
    private updateValues$: Subscription;

    @ViewChild('overlay', { read: ElementRef }) overlay: ElementRef<HTMLDivElement>;
    @ViewChild('svgElement', { read: ElementRef }) svg: ElementRef<SVGElement>;
    @ViewChild('imgContainer', { read: ElementRef }) imgContainer: ElementRef<HTMLDivElement>;

    isLoading = true;
    selected: number;
    mnemoscheme: Mnemoscheme[];
    deviceItemGroup: Device_Item_Group;
    digOverlayPosition: { x: number; y: number };

    constructor(
        private controlService: ControlService,
        private schemeService: SchemeService,
        private changeDetectorRef: ChangeDetectorRef,
    ) {
    }

    ngOnInit(): void {
        this.schemeService.getMnemoscheme()
            .subscribe(r => {
                this.mnemoscheme = r;
                this.isLoading = false;
                if (this.mnemoscheme.length > 0) {
                    this.select(this.mnemoscheme[0].id);
                }
            });
    }

    select(id: number) {
        if (this.updateValues$) {
            this.updateValues$.unsubscribe();
        }

        this.selected = id;

        this.schemeService.getMnemoschemeImage(id)
            .subscribe((text) => {
                this.svg.nativeElement.innerHTML = text;
                setTimeout(() => {
                    this.bindClickEvents();
                    this.adjustSvgHeight();
                    this.updateValues();
                    this.updateValues$ = this.controlService.dev_item_changed.subscribe(() => this.updateValues());
                }, 4);
            });
    }

    private bindClickEvents() {
        const imgItems = document.querySelectorAll('[data-dig-id]');
        imgItems.forEach((elem) => {
            elem.addEventListener('click', (ev: MouseEvent) => {
                ev.preventDefault();
                ev.stopPropagation();
                const digId = +elem.getAttribute('data-dig-id');

                this.showDig(digId, {
                    x: ev.x,
                    y: ev.y,
                });
            });
        });
    }

    private updateValues() {
        // console.group('#updateValues');
        const imgItems = document.querySelectorAll('[data-device-item-id]');
        imgItems.forEach((elem) => {
            const devItemId = +elem.getAttribute('data-device-item-id');
            const isCssAnim = elem.hasAttribute('data-css-anim-if-value');
            const isSmilAnim = elem.hasAttribute('data-smil-anim-if-value');
            const isShow = elem.hasAttribute('data-show-if-value');
            const isValueBinding = elem.hasAttribute('data-value');

            const devItem = this.schemeService.devItemById(devItemId);
            const { value } = devItem.val;

            // console.log(devItem.name || devItem.type.title || devItem.type.name, value);

            if (isCssAnim) {
                const cssAnimEnableValue = elem.getAttribute('data-css-anim-if-value');
                if (MnemoschemeComponent.translateValue(cssAnimEnableValue) == value) {
                    // console.log('Start CssAnim');
                    MnemoschemeComponent.startCssAnimation(elem);
                } else {
                    // console.log('Stop CssAnim');
                    MnemoschemeComponent.stopCssAnimation(elem);
                }
            }

            if (isSmilAnim) {
                const smilAnimEnableValue = elem.getAttribute('data-smil-anim-if-value');
                if (MnemoschemeComponent.translateValue(smilAnimEnableValue) == value) {
                    // console.log('Start SMIL');
                    MnemoschemeComponent.startSmilAnimation(elem);
                } else {
                    // console.log('Stop SMIL');
                    MnemoschemeComponent.stopSmilAnimation(elem);
                }
            }

            if (isShow) {
                const showIfValue = elem.getAttribute('data-show-if-value');
                if (MnemoschemeComponent.translateValue(showIfValue) == value) {
                    // console.log('Show');
                    MnemoschemeComponent.showElement(elem);
                } else {
                    // console.log('Hide');
                    MnemoschemeComponent.hideElement(elem);
                }
            }

            if (isValueBinding) {
                let binding = '';
                if (!value) {
                    binding = '?';
                } else {
                    const sign = devItem.type.sign?.name || '';
                    binding = `${value}${sign}`;
                }

                elem.innerHTML = binding;
            }
        });

        // console.groupEnd();
    }

    private static startCssAnimation(elem: Element) {
        const animations = elem.getAnimations();
        animations.forEach(anim => {
            if (anim.playState !== 'running') {
                anim.play();
            }
        });
    }

    private static stopCssAnimation(elem: Element) {
        const animations = elem.getAnimations();
        animations.forEach(anim => anim.cancel());
    }

    private static startSmilAnimation(elem: Element) {
        elem.childNodes.forEach((child) => {
            if (child.nodeName.indexOf('animate') >= 0) {
                const anim = child as SVGAnimationElement;
                if (!anim.hasAttribute('data-anim-started')) {
                    anim.setAttribute('data-anim-started', '1');
                    (<any>anim).beginElement();
                }
            }
        });
    }

    private static stopSmilAnimation(elem: Element) {
        elem.childNodes.forEach((child) => {
            if (child.nodeName.indexOf('animate') >= 0) {
                const anim = child as SVGAnimationElement;
                if (anim.hasAttribute('data-anim-started')) {
                    anim.removeAttribute('data-anim-started');
                }
                (<any>anim).endElement();
            }
        });
    }

    private static showElement(elem: Element) {
        elem.classList.remove('hidden');
    }

    private static hideElement(elem: Element) {
        elem.classList.add('hidden');
    }

    private showDig(digId: number, showParams: { x: number, y: number }) {
        this.digOverlayPosition = showParams;
        this.deviceItemGroup = this.schemeService
            .scheme.section[0].groups.find(group => group.id === digId);

        const containerHeight = this.imgContainer.nativeElement.clientHeight;
        const overflowX = this.digOverlayPosition.x + 350 - this.imgContainer.nativeElement.clientWidth;
        if (overflowX > 0) {
            this.digOverlayPosition.x -= overflowX;
        }

        this.changeDetectorRef.detectChanges();

        const overflowY = this.overlay.nativeElement.clientHeight
            + this.digOverlayPosition.y
            - containerHeight;

        if (overflowY > 0) {
            this.digOverlayPosition.y -= overflowY;
            if (this.digOverlayPosition.y < this.imgContainer.nativeElement.offsetTop) {
                this.digOverlayPosition.y = this.imgContainer.nativeElement.offsetTop;
            }

            this.changeDetectorRef.detectChanges();
        }
    }

    adjustSvgHeight() {
        const rect = this.imgContainer.nativeElement.getBoundingClientRect();
        const h = (<any>window).innerHeight - rect.y - 7;
        const w = (<any>window).innerWidth - rect.x - 12;
        const svg = this.svg.nativeElement.firstElementChild as SVGElement;
        svg.style.height = `${h}px`;
        svg.style.width = `${w}px`;
    }

    private static translateValue(value: string): any {
        if (value === 'null') {
            return null;
        }

        return value;
    }

    imgContainerClick() {
        this.deviceItemGroup = null;
    }
}
