import {Component, OnInit} from '@angular/core';
import {ActivatedRoute} from '@angular/router';
import {FormControl} from '@angular/forms';
import {Observable, Subject} from 'rxjs';
import {startWith, map} from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { debounceTime } from 'rxjs/operators';
import { distinctUntilChanged } from 'rxjs/operators';
import { takeUntil } from 'rxjs/operators';
import { Router} from '@angular/router';
import { FormGroup } from '@angular/forms';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css']
})

export class SpaceAddComponent implements OnInit {

	createForm = new FormGroup({
		name: new FormControl(''),
		key: new FormControl(''),
	  });

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

	keyWasChanged: boolean = false;

	constructor(
		private route: ActivatedRoute,
		private router: Router
	) {
	}

	private translitFromRuToEn = {"Ё":"YO","Й":"I","Ц":"TS","У":"U","К":"K","Е":"E","Н":"N","Г":"G","Ш":"SH","Щ":"SCH","З":"Z","Х":"H","Ъ":"'","ё":"yo","й":"i","ц":"ts","у":"u","к":"k","е":"e","н":"n","г":"g","ш":"sh","щ":"sch","з":"z","х":"h","ъ":"'","Ф":"F","Ы":"I","В":"V","А":"A","П":"P","Р":"R","О":"O","Л":"L","Д":"D","Ж":"ZH","Э":"E","ф":"f","ы":"i","в":"v","а":"a","п":"p","р":"r","о":"o","л":"l","д":"d","ж":"zh","э":"e","Я":"Ya","Ч":"CH","С":"S","М":"M","И":"I","Т":"T","Ь":"'","Б":"B","Ю":"YU","я":"ya","ч":"ch","с":"s","м":"m","и":"i","т":"t","ь":"'","б":"b","ю":"yu"};

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

	onSelectOption(option: MatOption) {
		if (option){
			this.selectedSpace = option.value;
		}		
	}

	sendRequestToJoin(){
		this.router.navigate(['space/add/request/' + this.selectedSpace]);
	}

	private _transliterate(word: string): string{
		let self = this;
		return word.split('').map(function (char) { 
		  return self.translitFromRuToEn[char] || char; 
		}).join("");
  	}

	onNameChange(name: string) {
		if (!this.keyWasChanged){
			let newKey: string = this._transliterate(name);
			newKey = newKey.replace(/[^a-z0-9_]/g, '');
			this.createForm.patchValue({
				key : newKey
			   });
		}
	}

	onKeyChange(key: string){
		this.keyWasChanged = true;
	}
}
