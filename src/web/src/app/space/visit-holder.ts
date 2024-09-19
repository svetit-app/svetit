import {Injectable} from '@angular/core';

@Injectable()
export class SpaceVisitHolder {
	private _key = 'spacePath';

	get() : string {
		return localStorage.getItem(this._key);
	}

	set(spacePath: string) {
		localStorage.setItem(this._key, spacePath);
	}

	clear() {
		localStorage.removeItem(this._key);
	}
}
