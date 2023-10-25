import {Component, Inject, OnInit} from '@angular/core';
import {MAT_DIALOG_DATA, MatDialogRef} from '@angular/material/dialog';

@Component({
  selector: 'app-confirmation-dialog',
  templateUrl: './confirmation-dialog.component.html',
  styleUrls: ['./confirmation-dialog.component.css']
})
export class ConfirmationDialogComponent {

  constructor(
      @Inject(MAT_DIALOG_DATA) public dialogData: { text?: string },
      public dialogRef: MatDialogRef<ConfirmationDialogComponent>,
  ) { }
}
