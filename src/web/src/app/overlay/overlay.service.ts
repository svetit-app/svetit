import { Injectable, TemplateRef, ViewContainerRef } from '@angular/core';
import { Overlay, OverlayConfig, OverlayRef, PositionStrategy } from '@angular/cdk/overlay';
import { TemplatePortal } from '@angular/cdk/portal';
import { Observable, throwError } from 'rxjs';
import { tap, catchError } from 'rxjs/operators';

@Injectable()
export class OverlayService {
	public displayProgressSpinner = false;
	appSpinner: TemplateRef<any>;
	private progressSpinnerOverlayConfig: OverlayConfig;
	private overlayRef: OverlayRef;
	private vcRef: ViewContainerRef;

	constructor(
		private overlay: Overlay, 
	) { }

	createOverlay(config: OverlayConfig): OverlayRef {
		return this.overlay.create(config);
	}

	positionGloballyCenter(): PositionStrategy {
		return this.overlay.position()
			.global()
			.centerHorizontally()
			.centerVertically();
	}

	setProgressSpinner(appSpinner: TemplateRef<any>, vcRef: ViewContainerRef) {
		this.appSpinner = appSpinner;
		this.vcRef = vcRef;

		this.progressSpinnerOverlayConfig = {
			hasBackdrop: true,
			positionStrategy: this.positionGloballyCenter()
		};
		// Create Overlay for progress spinner
		this.overlayRef = this.createOverlay(this.progressSpinnerOverlayConfig);
	}

	public showSpinner(){
		if (this.overlayRef.hasAttached())
			return;
		let templatePortal = new TemplatePortal(this.appSpinner, this.vcRef);
		this.overlayRef.attach(templatePortal);
	}

	public hideSpinner(){
		if (!this.overlayRef.hasAttached())
			return;
		this.overlayRef.detach();
	}

	WatchFor<T>(source$: Observable<T>): Observable<T> {
		this.showSpinner();
		return source$.pipe(
			tap(v => this.hideSpinner()),
			catchError(err => {
			  this.hideSpinner();
			  this.showError(err.message);
			  console.warn('catch', err);
			  return throwError(err);
		  }),
		)
	}

	showError(msg: string){
		alert(msg);
	}
}