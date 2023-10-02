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

	constructor(
		private route: ActivatedRoute,
		private router: Router,
		private fb: FormBuilder,
	) {
		this._createForm();
	}

	private translitFromRuToEn = {
	"Ё": "YO", "Й": "I", "Ц": "TS", "У": "U", "К": "K", "Е": "E", "Н": "N", "Г": "G", "Ш": "SH", "Щ": "SCH",
	"З": "Z", "Х": "H", "Ъ": "'", "ё": "yo", "й": "i", "ц": "ts", "у": "u", "к": "k", "е": "e", "н": "n", "г": "g",
	"ш": "sh", "щ": "sch", "з": "z", "х": "h", "ъ": "'", "Ф": "F", "Ы": "I", "В": "V", "А": "A", "П": "P", "Р": "R",
	"О": "O", "Л": "L", "Д": "D", "Ж": "ZH", "Э": "E", "ф": "f", "ы": "i", "в": "v", "а": "a", "п": "p", "р": "r",
	"о": "o", "л": "l", "д": "d", "ж": "zh", "э": "e", "Я": "Ya", "Ч": "CH", "С": "S", "М": "M", "И": "I", "Т": "T",
	"Ь": "'", "Б": "B", "Ю": "YU", "я": "ya", "ч": "ch", "с": "s", "м": "m", "и": "i", "т": "t", "ь": "'", "б": "b",
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
			this.selectedSpace = option.value;
		}
	}

	sendRequestToJoin() {
		if (this.selectedSpace) {
			this.router.navigate(['space/add/request/' + this.selectedSpace.key]);
		}
	}

	private _transliterate(word: string): string {
		let self = this;
		return word.split('').map(function (char) {
			return self.translitFromRuToEn[char] || char;
		}).join("");
	}

	onNameChange(name: string) {
		if (!this.keyWasChanged) {
			let newKey: string = this._transliterate(name);
			newKey = newKey.replace(/[^a-zA-Z0-9_]/g, '');
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
			console.log("form is invalid");
			return;
		}
		let contains: boolean = this.spacesWithKeys.some(e => e.key === data.key);

		if (!contains) {
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
			console.log("space with key added");
		} else {
			this.createForm.controls['key'].setErrors({ 'incorrect': true });
			console.log("space with such key exists");
		}
	}

	private _createForm() {
		this.createForm = this.fb.group({
			name: ['', [Validators.required]],
			key: ['', [
				Validators.required,
				Validators.pattern('[a-zA-Z0-9_]*'),
			],],
		});
	}
}
