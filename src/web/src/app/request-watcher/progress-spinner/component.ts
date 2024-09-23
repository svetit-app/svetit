import { Component, Input, OnInit, ViewChild, TemplateRef, ViewContainerRef, DoCheck, inject } from '@angular/core';
import { RequestWatcherService } from '../service';

@Component({
    selector: 'app-progress-spinner',
    templateUrl: './component.html',
    styleUrls: ['./component.css'],
    standalone: true
})
export class ProgressSpinnerComponent {
	private vcRef = inject(ViewContainerRef);
	private requestWatcher = inject(RequestWatcherService);

	@ViewChild('progressSpinnerRef')
	private progressSpinnerRef: TemplateRef<any>;

	ngAfterViewInit() {
		this.requestWatcher.SetProgressSpinner(this.progressSpinnerRef, this.vcRef);
	}
}