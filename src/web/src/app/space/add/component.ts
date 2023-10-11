import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Observable, Subject, of} from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, takeUntil, switchMap } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { Router, NavigationExtras } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';
import { SpaceService } from '../service';

import { Space } from '../model';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css'],
})

export class SpaceAddComponent implements OnInit {
	createForm: FormGroup;
	controlAutocomplete = new FormControl('');

	spaces: Space[] = [];

	filteredSpaces: Observable<Space[]>;
	selectedSpace: Space;
	private readonly ngUnsubscribe = new Subject();
	keyWasChanged: boolean = false;

	displayProgressSpinner = false;

	constructor(
		private router: Router,
		private fb: FormBuilder,
		private space: SpaceService
	) {
		this._createForm();
	}

	private translitFromRuToEn = {
	"ё": "yo", "й": "i", "ц": "ts", "у": "u", "к": "k", "е": "e", "н": "n", "г": "g",
	"ш": "sh", "щ": "sch", "з": "z", "х": "h", "ъ": "'", "ф": "f", "ы": "i", "в": "v", "а": "a", "п": "p", "р": "r", "о": "o", "л": "l", "д": "d", "ж": "zh", "э": "e", "я": "ya", "ч": "ch", "с": "s", "м": "m", "и": "i", "т": "t", "ь": "'", "б": "b",
	"ю": "yu"
	};

	ngOnInit() {

		this.filteredSpaces = this.controlAutocomplete.valueChanges.pipe(
			startWith(''),
			debounceTime(300), // Optional: debounce input changes to avoid excessive requests
			distinctUntilChanged(), // Optional: ensure distinct values before making requests
			switchMap(value => this.space.getSpaceListWith10ItemsByName(value || '').pipe(
				map(apiResponse => apiResponse.results)
			))
		);

		this.controlAutocomplete.valueChanges.pipe(
			debounceTime(500),
			distinctUntilChanged(),
			takeUntil(this.ngUnsubscribe)
		).subscribe(selected => {
			if (!selected) {
				this.selectedSpace = null;
			}
		});
	}

	onSelectOption(option: MatOption) {
		if (option) {
			this.selectedSpace = {
				id: option.value.id,
				name: option.value.name,
				key: option.value.key,
				requestsAllowed: option.value.requestAllowed,
				createdAt: option.value.createdAt
			}
		}
	}

	sendRequestToJoin() {
		const navigationExtras: NavigationExtras = {state: {spaceName: this.selectedSpace.name}};
		this.router.navigate(['space/add/request'], navigationExtras);
	}

	private _transliterate(word: string): string {
		return word
			.split('')
			.map(char => this.translitFromRuToEn[char] || char)
			.join('');
	}

	onNameChange(name: string) {
		if (!this.keyWasChanged) {
			let newKey = name.toLowerCase();
			newKey = this._transliterate(newKey);
			newKey = newKey.replace(/[^a-z0-9_]/g, '');
			this.createForm.patchValue({
				key: newKey
			});
		}
	}

	onKeyChange(key: string) {
		this.keyWasChanged = true;
	}

	onSubmitCreate(data): void {
		if (this.createForm.invalid) {
			return;
		}
		this.showProgressSpinner();
		let contains = false;
		this.space.isExists(data.key).subscribe(res => {
			if (res) {
				contains = true;
			}
		});

		if (!contains) {
			this.space.createNew(data.name, data.key, data.requestsAllowed)
				.subscribe(res => { 
					if (res != true){
						alert("error!");
					}
					this.hideProgressSpinner();
					this.router.navigate(['space/list']);
				});
		} else {
			this.createForm.controls['key'].setErrors({ 'incorrect': true });
		}
	}

	private _createForm() {
		this.createForm = this.fb.group({
			name: ['', [Validators.required]],
			key: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			],],
			requestsAllowed: [false, [Validators.required]],
		});
	}

	displaySpaceName(value) {
		return value?.name;
	}

	showProgressSpinner() {
		this.displayProgressSpinner = true;
	};

	hideProgressSpinner() {
		this.displayProgressSpinner = false;
	};
	
}
