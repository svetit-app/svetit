import {ChangeDetectorRef, Directive, Input, OnInit} from '@angular/core';
import {MatPaginator, PageEvent} from '@angular/material/paginator';

@Directive({
  selector: '[appRememberPageLimit]'
})
export class RememberPageLimitDirective implements OnInit {
	@Input('appRememberPageLimit') localStorageFieldName: string;

	constructor(private elementRef: MatPaginator) {
	}

	ngOnInit() {
		const limit = this.load();
		if (limit) {
			this.elementRef.pageSize = limit;
			this.elementRef.page.emit({
				length: this.elementRef.length,
				pageIndex: this.elementRef.pageIndex,
				pageSize: limit,
			});
		}

		this.elementRef.page.subscribe((event: PageEvent) => {
			this.store(event.pageSize);
		});
	}

	load(): number {
		const val = localStorage.getItem(this.localStorageFieldName);
		return val ? parseInt(val, 10) : null;
	}

	store(v: number) {
		localStorage.setItem(this.localStorageFieldName, v.toString(10));
	}
}
