import { Component, Input, OnInit, ViewChild, TemplateRef, ViewContainerRef, DoCheck } from '@angular/core';
import { OverlayRef } from '@angular/cdk/overlay';

import { OverlayConfig } from '@angular/cdk/overlay'; 
import { OverlayService } from '../overlay/overlay.service';

@Component({
	selector: 'app-progress-spinner',
	templateUrl: './progress-spinner.component.html',
	styleUrls: ['./progress-spinner.component.css']
})
export class ProgressSpinnerComponent {
	@Input() backdropEnabled = true;
	@Input() positionGloballyCenter = true;
	@Input() displayProgressSpinner: boolean;

	@ViewChild('progressSpinnerRef')
	private progressSpinnerRef: TemplateRef<any>;
	private progressSpinnerOverlayConfig: OverlayConfig;
	private overlayRef: OverlayRef;
	
	constructor(private vcRef: ViewContainerRef, private overlayService: OverlayService) { }

	ngOnInit() {
		// Config for Overlay Service
		this.progressSpinnerOverlayConfig = {
			hasBackdrop: this.backdropEnabled
		};
		if (this.positionGloballyCenter) {
			this.progressSpinnerOverlayConfig['positionStrategy'] = this.overlayService.positionGloballyCenter();
		}
		// Create Overlay for progress spinner
		this.overlayRef = this.overlayService.createOverlay(this.progressSpinnerOverlayConfig);
	}

	ngDoCheck() {
		// Based on status of displayProgressSpinner attach/detach overlay to progress spinner template
		if (this.displayProgressSpinner && !this.overlayRef.hasAttached()) {
			this.overlayService.attachTemplatePortal(this.overlayRef, this.progressSpinnerRef, this.vcRef);
		} else if (!this.displayProgressSpinner && this.overlayRef.hasAttached()) {
			this.overlayRef.detach();
		}
	}
}