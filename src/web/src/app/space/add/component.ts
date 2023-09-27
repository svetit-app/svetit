import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';
import {FormControl} from '@angular/forms';
import {Observable, Subject} from 'rxjs';
import {startWith, map} from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { debounceTime } from 'rxjs/operators';
import { distinctUntilChanged } from 'rxjs/operators';
import { takeUntil } from 'rxjs/operators';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})

export class SpaceAddComponent implements OnInit {

	control = new FormControl('');
  	spaces: string[] = [
		'Пространство №1 ABC',
		'Пространство №2 DEF',
		'Пространство №3 GHI',
		'Пространство №4 JKL',
		'Пространство №5 MNO',
		'Пространство №6 PQR',
		'Пространство №7 STU',
		'Пространство №8 VWX',
		'Пространство №9 YZA',
		'Пространство №10 BCD',
		'Пространство №11 EFG',
		'Пространство №12 HIJ',
		'Пространство №13 KLM',
		'Пространство №14 NOP',
		'Пространство №15 QRS',
	];
  	filteredSpaces: Observable<string[]>;
	selectedSpace: string;
	private readonly ngUnsubscribe = new Subject();

	constructor(
		private route: ActivatedRoute,
	) {
	}

	ngOnInit() {
		this.filteredSpaces = this.control.valueChanges.pipe(
			startWith(''),
			map(value => this._filter(value || '')),
		);

		this.control.valueChanges.pipe(
			debounceTime(500),
			distinctUntilChanged(),
			takeUntil(this.ngUnsubscribe)
		)
		.subscribe(selected => {
			if (!selected) {
				this.selectedSpace = "";
			}
		});
	}
	private _filter(value: string): string[] {
		const filterValue = this._normalizeValue(value);
		return this.spaces.filter(space => this._normalizeValue(space).includes(filterValue)).slice(0,10);
	}
	
	private _normalizeValue(value: string): string {
		return value.toLowerCase().replace(/\s/g, '');
	}

	OnSelected(option: MatOption) {
		if (option){
			this.selectedSpace = option.value;
		}		
	}
	showAlert(){
		alert("Запрос на присоединение к пространству \"" + this.selectedSpace + "\" отправлен.");
	}
}
