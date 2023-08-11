import { Component, OnInit } from '@angular/core';
import {MatLegacyDialogRef as MatDialogRef} from '@angular/material/legacy-dialog';

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
