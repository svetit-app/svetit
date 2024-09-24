import { Component, OnInit } from '@angular/core';
import { SchemeGroupsListComponent } from '../scheme-groups-list/scheme-groups-list.component';
import { RouterOutlet } from '@angular/router';

@Component({
    selector: 'app-scheme-groups',
    templateUrl: './scheme-groups.component.html',
    styleUrls: ['./scheme-groups.component.css'],
    standalone: true,
    imports: [SchemeGroupsListComponent, RouterOutlet]
})
export class SchemeGroupsComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
