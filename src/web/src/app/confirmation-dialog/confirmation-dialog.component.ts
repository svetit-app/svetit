import { Component, OnInit, inject } from '@angular/core';
import {MAT_DIALOG_DATA, MatDialogRef} from '@angular/material/dialog';

@Component({
    selector: 'app-confirmation-dialog',
    templateUrl: './confirmation-dialog.component.html',
    styleUrls: ['./confirmation-dialog.component.css'],
    standalone: true
})
export class ConfirmationDialogComponent {
  dialogData = inject(MAT_DIALOG_DATA);
  dialogRef = inject<MatDialogRef<ConfirmationDialogComponent>>(MatDialogRef);
}
