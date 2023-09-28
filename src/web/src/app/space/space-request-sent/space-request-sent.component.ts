import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';

@Component({
  selector: 'app-space-request-sent',
  templateUrl: './space-request-sent.component.html',
  styleUrls: ['./space-request-sent.component.css']
})
export class SpaceRequestSentComponent {

	space: string;

	constructor(private route: ActivatedRoute) {}

	ngOnInit(): void {
        this.space = this.route.snapshot.paramMap.get('id');
    }
}
