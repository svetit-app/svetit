import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { FormControl } from '@angular/forms';
import { Observable, Subject, of } from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, takeUntil } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

interface Space {
	name: string;
	key: string;
}

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css'],
})

export class SpaceAddComponent implements OnInit {
	createForm: FormGroup;
	controlAutocomplete = new FormControl('');
	spacesWithKeys: Space[] = [];
	filteredSpaces: Observable<Space[]>;
	selectedSpace: Space;
	private readonly ngUnsubscribe = new Subject();
	keyWasChanged: boolean = false;
	creatingSpace: boolean = false;

	constructor(
		private route: ActivatedRoute,
		private router: Router,
		private fb: FormBuilder,
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
			map(value => this._filter(value || '')),
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

	private _filter(value: string): Space[] {
		const filterValue = this._normalizeValue(value);
		return this.spacesWithKeys.filter(space => this._normalizeValue(space.name).includes(filterValue)).slice(0, 10);
	}

	private _normalizeValue(value: string): string {
		return value.toLowerCase().replace(/\s/g, '');
	}

	onSelectOption(option: MatOption) {
		if (option) {
			this.selectedSpace = {
				name: option.value.name,
				key: option.value.key,
			}
		}
	}

	sendRequestToJoin() {
		this.router.navigate(['space/add/request/' + this.selectedSpace.name]);
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
		let contains: boolean = this.spacesWithKeys.some(e => e.key === data.key);

		if (!contains) {
			this.creatingSpace = true;
			let spaceWithKey: Space = {
				name: data.name,
				key: data.key
			};
			this.spacesWithKeys.push(spaceWithKey);
			const filterValue = this._normalizeValue(this.controlAutocomplete.value || '');
			const filtered = this.spacesWithKeys.filter(space =>
				this._normalizeValue(space.name).includes(filterValue)
			).slice(0, 10);
			this.filteredSpaces = of(filtered);
			let self = this;
			setTimeout(function(self) {
				self.creatingSpace = false;
				self.router.navigate(['space/list']);
			}, 2000, self);
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
		});
	}

	displaySpaceName(value) {
		if (value) {
			return value.name;
		}
	}
	
}
