import { Component, Input, OnInit, ViewChild, TemplateRef, ViewContainerRef, DoCheck } from '@angular/core';
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';
import { RequestWatcherService } from '../service';

@Component({
	selector: 'app-progress-spinner',
	standalone: true,
	imports: [MatProgressSpinnerModule],
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
		this.requestWatcher.SetProgressSpinner(this.progressSpinnerRef, this.vcRef);
	}
}
