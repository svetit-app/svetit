import {Component, OnInit, Input, ViewChild, OnChanges, SimpleChanges, DoCheck, ChangeDetectorRef} from '@angular/core';
import { MatPaginator } from '@angular/material/paginator';
import { MatTooltip } from '@angular/material/tooltip';
import { MatDialog } from '@angular/material/dialog';

import {Device_Item_Group, DIG_Status_Info} from '../scheme';
import { StatusManageDialogComponent } from '../status-manage-dialog/status-manage-dialog.component';

@Component({
  selector: 'app-group-status',
  templateUrl: './group-status.component.html',
  styleUrls: ['./group-status.component.css']
})
export class GroupStatusComponent implements OnInit {

  @Input() group: Device_Item_Group;

  @ViewChild(MatTooltip, {static: true}) tooltip;

  get color(): string {
    if (this.group.status_info === undefined)
      return 'gray';
    return this.group.status_info.color;
  }

  get text(): string {
    if (this.group.status_info === undefined)
      return '?';
    return this.group.status_info.text;
  }

  get short_text(): string {
    if (this.group.status_info === undefined)
      return '?';
    return this.group.status_info.short_text;
  }

  constructor(
    private ref: ChangeDetectorRef,
      public dialog: MatDialog
  ) { }

  ngOnInit() {
    //console.log(this.group);
  }

  /*
  ngOnChanges(changes: SimpleChanges): void {
    //console.log(changes);
  }

  ngDoCheck() {
    if (this.group.status_info !== this.old_status_info) {
      console.log('a');
      this.ref.markForCheck();
    }
  }
  */

    openStatusManageDialog(): void {
        let dialogRef = this.dialog.open(StatusManageDialogComponent, {
          width: '100%',
          data: this.group
        });

        dialogRef.afterClosed().subscribe();
    }

  showStatusText(evnt: any): void {
    evnt.stopPropagation();
    this.tooltip.show();
  }
}
