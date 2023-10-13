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
	
	constructor(
		private vcRef: ViewContainerRef,
		private overlay: OverlayService)
	{ }

	ngAfterViewInit() {
		this.overlay.setProgressSpinner(this.progressSpinnerRef, this.vcRef);
	}
}