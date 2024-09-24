import { Injectable, inject } from '@angular/core';
import {BreakpointObserver, BreakpointState} from '@angular/cdk/layout';
import {Observable} from 'rxjs';
import {MatDialog} from '@angular/material/dialog';
import {ConfirmationDialogComponent} from './confirmation-dialog/confirmation-dialog.component';

@Injectable()
export class UIService {
	breakpointObserver = inject(BreakpointObserver);
	private dialog = inject(MatDialog);


	scrollTop_ = 0;
	toolBarHidden_ = false;
	lang = 'ru';

	get scrollTop(): number {
		return this.scrollTop_;
	}

	onScroll($event) {
		this.scrollTop_ = $event.target.scrollTop;
	}

	isToolbarHidden(): boolean {
		if (window.pageYOffset > 64 ) {
			this.toolBarHidden_ = true;
		}

		if (window.pageYOffset === 0) {
			this.toolBarHidden_ = false;
		}

		return this.toolBarHidden_; // TODO: Toolbar has different height on different window.width's
	}

	mobileBreakpointObserver(): Observable<BreakpointState> {
		return this.breakpointObserver
			.observe(['(min-width: 600px)'])
	}

	getCurLang() {
		return this.lang;
	}

	setCurLang(l: string) {
		this.lang = l;
	}

	confirmationDialog(text?: string): Observable<boolean> {
			return this.dialog.open(ConfirmationDialogComponent, { data: { text } })
					.afterClosed();
	}
}
