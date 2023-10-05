import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Observable, Subject, of } from 'rxjs';
import { startWith, map, debounceTime, distinctUntilChanged, takeUntil } from 'rxjs/operators';
import { MatOption } from '@angular/material/core';
import { Router, NavigationExtras } from '@angular/router';
import { FormGroup, FormBuilder, Validators } from '@angular/forms';

import { SpaceInterface } from '../model';

@Component({
	selector: 'app-space-add',
	templateUrl: './component.html',
	styleUrls: ['./component.css'],
})

export class SpaceAddComponent implements OnInit {
	createForm: FormGroup;
	controlAutocomplete = new FormControl('');

	// is ok to use crypto.randomUUID() for UUID generation? or it'd better to get something from npm for that?
	spaces: SpaceInterface[] = [
		{id: crypto.randomUUID(), name: "Пространство №1", key: "key1", requestsAllowed: true, createdAt: new Date("2023-10-01")},
		{id: crypto.randomUUID(), name: "Пространство №2", key: "key2", requestsAllowed: true, createdAt: new Date("2023-10-02")},
		{id: crypto.randomUUID(), name: "Пространство №3", key: "key3", requestsAllowed: true, createdAt: new Date("2023-10-03")},
		{id: crypto.randomUUID(), name: "Пространство №4", key: "key4", requestsAllowed: true, createdAt: new Date("2023-10-04")},
		{id: crypto.randomUUID(), name: "Пространство №5", key: "key5", requestsAllowed: true, createdAt: new Date("2023-10-05")},
		{id: crypto.randomUUID(), name: "Пространство №6", key: "key6", requestsAllowed: true, createdAt: new Date("2023-10-06")},
		{id: crypto.randomUUID(), name: "Пространство №7", key: "key7", requestsAllowed: true, createdAt: new Date("2023-10-07")},
		{id: crypto.randomUUID(), name: "Пространство №8", key: "key8", requestsAllowed: true, createdAt: new Date("2023-10-08")},
		{id: crypto.randomUUID(), name: "Пространство №9", key: "key9", requestsAllowed: true, createdAt: new Date("2023-10-09")},
		{id: crypto.randomUUID(), name: "Пространство №10", key: "key10", requestsAllowed: true, createdAt: new Date("2023-10-10")},
		{id: crypto.randomUUID(), name: "Пространство №11", key: "key11", requestsAllowed: true, createdAt: new Date("2023-10-11")},
		{id: crypto.randomUUID(), name: "Пространство №12", key: "key12", requestsAllowed: true, createdAt: new Date("2023-10-12")},
		{id: crypto.randomUUID(), name: "Пространство №13", key: "key13", requestsAllowed: true, createdAt: new Date("2023-10-13")},
		{id: crypto.randomUUID(), name: "Пространство №14", key: "key14", requestsAllowed: true, createdAt: new Date("2023-10-14")},
		{id: crypto.randomUUID(), name: "Пространство №15", key: "key15", requestsAllowed: true, createdAt: new Date("2023-10-15")},
	];

	filteredSpaces: Observable<SpaceInterface[]>;
	selectedSpace: SpaceInterface;
	private readonly ngUnsubscribe = new Subject();
	keyWasChanged: boolean = false;
	creatingSpace: boolean = false;

	constructor(
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
		// maybe it's possible to unity this two pipes in one?
		this.filteredSpaces = this.controlAutocomplete.valueChanges.pipe(
			startWith(''),
			map(value => this._filter(value || ''))
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

	private _filter(value: string): SpaceInterface[] {
		const filterValue = this._normalizeValue(value);
		return this.spaces.filter(space => this._normalizeValue(space.name).includes(filterValue)).slice(0, 10);
	}

	private _normalizeValue(value: string): string {
		return value.toString().toLowerCase().replace(/\s/g, '');
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
		// is it ok to use space key for searching an unique space?
		let contains: boolean = this.spaces.some(e => e.key === data.key);

		if (!contains) {
			this.creatingSpace = true;
			let newSpace: SpaceInterface = {
				id: data.id,
				name: data.name,
				key: data.key,
				requestsAllowed: data.requestsAllowed,
				createdAt: data.createdAt
			};
			this.spaces.push(newSpace);
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
			id: [crypto.randomUUID(), [Validators.required]],
			name: ['', [Validators.required]],
			key: ['', [
				Validators.required,
				Validators.pattern('[a-z0-9_]*'),
			],],
			requestsAllowed: [false, [Validators.required]],
			createdAt: [Date(), [Validators.required]]
		});
	}

	displaySpaceName(value) {
		if (value) {
			return value.name;
		}
	}
	
}
