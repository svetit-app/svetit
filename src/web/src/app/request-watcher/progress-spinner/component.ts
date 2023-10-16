import { Component, Input, OnInit, ViewChild, TemplateRef, ViewContainerRef, DoCheck } from '@angular/core';
import { RequestWatcherService } from '../service';

@Component({
	selector: 'app-progress-spinner',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class ProgressSpinnerComponent {
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