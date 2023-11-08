import { Injectable, TemplateRef, ViewContainerRef } from '@angular/core';
import { Overlay, OverlayConfig, OverlayRef, PositionStrategy } from '@angular/cdk/overlay';
import { TemplatePortal } from '@angular/cdk/portal';
import { Observable, throwError } from 'rxjs';
import { tap, catchError } from 'rxjs/operators';

@Injectable()
export class RequestWatcherService {
	spinner: TemplateRef<any>;
	private overlayRef: OverlayRef;
	private vcRef: ViewContainerRef;

	constructor(
		private overlay: Overlay,
	) { }

	PositionGloballyCenter(): PositionStrategy {
		return this.overlay.position()
			.global()
			.centerHorizontally()
			.centerVertically();
	}

	SetProgressSpinner(spinner: TemplateRef<any>, vcRef: ViewContainerRef) {
		this.spinner = spinner;
		this.vcRef = vcRef;

		// Create Overlay for progress spinner
		this.overlayRef = this.overlay.create({
			hasBackdrop: true,
			positionStrategy: this.PositionGloballyCenter()
		});
	}

	ShowSpinner(){
		if (this.overlayRef.hasAttached())
			return;
		let templatePortal = new TemplatePortal(this.spinner, this.vcRef);
		this.overlayRef.attach(templatePortal);
	}

	HideSpinner(){
		if (!this.overlayRef.hasAttached())
			return;
		this.overlayRef.detach();
	}

	WatchFor<T>(source$: Observable<T>): Observable<T> {
		this.ShowSpinner();
		return source$.pipe(
			tap(v => this.HideSpinner()),
			catchError(err => {
				this.HideSpinner();
				this.ShowError(err.message);
				return throwError(err);
			}),
		)
	}

	ShowError(msg: string){
		console.warn('catch', msg);
	}
}
