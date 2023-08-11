import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {ProgressBarMode} from '@angular/material/progress-bar';
import {ThemePalette} from '@angular/material/core';
import {MatLegacySnackBar as MatSnackBar} from '@angular/material/legacy-snack-bar';

export class LoadingProgressbar {

    constructor(
        private snackBar: MatSnackBar,
        private changeDetectorRef: ChangeDetectorRef,
    ) {
    }

    loading: boolean;
    showProgressBar: boolean;
    progressBarMode: ProgressBarMode;
    progressBarColor: ThemePalette;
    progressBarValue: number;

    startLoading() {
        this.loading = true;

        this.showProgressBar = true;
        this.progressBarMode = 'indeterminate';
        this.progressBarColor = 'primary';

        this.changeDetectorRef.detectChanges();
    }

    finishedLoading() {
        if (!this.loading) {
            return;
        } // just for safety

        this.loading = false;
        this.progressBarColor = 'primary';
        this.progressBarMode = 'determinate';
        this.progressBarValue = 50;
        this.setProgressBarValueTimeout(100);

        this.changeDetectorRef.detectChanges();

        setTimeout(() => this.hideProgressBar(), 600);
    }

    errorLoading(error: Error) {
        this.loading = false;
        this.progressBarColor = 'warn';
        this.progressBarMode = 'determinate';
        this.setProgressBarValueTimeout(100);

        this.changeDetectorRef.detectChanges();

        setTimeout(() => {
            this.hideProgressBar();
            this.showLoadingError(error);
        }, 1000);
    }

    showLoadingError(error: Error) {
        this.snackBar.open(error.message, 'Hide', {
            duration: 10000,
            horizontalPosition: 'end',
            verticalPosition: 'bottom',
        });
    }

    hideProgressBar() {
        this.showProgressBar = false;
        this.changeDetectorRef.detectChanges();
    }

    private setProgressBarValueTimeout(number: number) {
        setTimeout(() => {
            this.progressBarValue = number;
            this.changeDetectorRef.detectChanges();
        }, 50);
    }
}
