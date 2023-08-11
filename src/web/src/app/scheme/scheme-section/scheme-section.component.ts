import {Component, Input, OnInit, Output, EventEmitter} from '@angular/core';
import {UIService} from '../../ui.service';
import {BreakpointState} from '@angular/cdk/layout';

@Component({
  selector: 'app-scheme-section',
  templateUrl: './scheme-section.component.html',
  styleUrls: ['./scheme-section.component.css']
})
export class SchemeSectionComponent implements OnInit {
  @Input() title: string;
  @Input() enableEditor: boolean;

  @Input() set forceExpand(isForceExpand: boolean) {
    if(isForceExpand) {
      this.isExpanded = isForceExpand;
    }
    this._isForceExpanded = isForceExpand;
  }

  @Output() edit: EventEmitter<void> = new EventEmitter();
  @Output() remove: EventEmitter<void> = new EventEmitter();

  _isForceExpanded: boolean;

  isExpanded = false;

  constructor(private uiService: UIService) { }

  ngOnInit() {
    // TODO: Make something more convenient
    this.uiService.mobileBreakpointObserver().subscribe((state: BreakpointState) => {
      if (state.matches) {
        // desktop
        this.isExpanded = true;
      } else {
        // mobile
        this.isExpanded = this._isForceExpanded;
      }
    });
  }

  toggleExpand() {
    this.isExpanded = !this.isExpanded;
    this._isForceExpanded = false;
  }

  onEdit($event) {
      $event.stopPropagation();
      $event.preventDefault();
      this.edit.emit();
  }

  onRemove($event) {
      $event.stopPropagation();
      $event.preventDefault();
      this.remove.emit();
  }
}
