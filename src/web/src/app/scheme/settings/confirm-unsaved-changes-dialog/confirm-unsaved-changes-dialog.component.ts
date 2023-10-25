import { Component, OnInit } from '@angular/core';
import {MatDialogRef} from '@angular/material/dialog';

@Component({
  selector: 'app-confirm-unsaved-changes-dialog',
  templateUrl: './confirm-unsaved-changes-dialog.component.html',
  styleUrls: ['./confirm-unsaved-changes-dialog.component.css']
})
export class ConfirmUnsavedChangesDialogComponent implements OnInit {

  constructor(public dialogRef: MatDialogRef<ConfirmUnsavedChangesDialogComponent>) { }

  ngOnInit(): void {
  }

}
