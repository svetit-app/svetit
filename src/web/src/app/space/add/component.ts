import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';
import {FormControl} from '@angular/forms';
import {Observable} from 'rxjs';
import {startWith, map} from 'rxjs/operators';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})
export class SpaceAddComponent implements OnInit {

	control = new FormControl('');
  	spaces: string[] = ['Пространство №1', 'Пространство №2', 'Пространство №3', 'Пространство №4'];
  	filteredSpaces: Observable<string[]>;

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
		this.filteredSpaces = this.control.valueChanges.pipe(
			startWith(''),
			map(value => this._filter(value || '')),
		);
	}

	private _filter(value: string): string[] {
		const filterValue = this._normalizeValue(value);
		return this.spaces.filter(space => this._normalizeValue(space).includes(filterValue));
	}
	
	private _normalizeValue(value: string): string {
		return value.toLowerCase().replace(/\s/g, '');
	}
}
