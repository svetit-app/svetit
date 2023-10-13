import { Component, Input, OnInit, ViewChild, TemplateRef, ViewContainerRef, DoCheck } from '@angular/core';
import { RequestWatcherService } from '../request-watcher/service';

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
	
	constructor(
		private vcRef: ViewContainerRef,
		private requestWatcher: RequestWatcherService)
	{ }

	ngAfterViewInit() {
		this.requestWatcher.setProgressSpinner(this.progressSpinnerRef, this.vcRef);
	}
}