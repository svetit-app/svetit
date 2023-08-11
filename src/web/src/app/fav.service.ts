import {Injectable} from '@angular/core';
import {CookieService} from 'ngx-cookie-service';

@Injectable()
export class FavService {

	favs: {
					name: string;
					title: string;
				}[];

	constructor(
		public cookie: CookieService,
	) {
		this.favs = this.getFavs();
	}

	getFavs() {
		const favStr = this.cookie.get('favs');

		if (!favStr) {
			this.favs = [];
		} else {
			this.favs = JSON.parse(favStr);
		}

		return this.favs;
	}

	setFavs() {
		const favStr = JSON.stringify(this.favs);
		this.cookie.set('favs', favStr, 365 * 5, '/');
	}

	addToFav(name: string, title: string) {
		this.favs.push({name: name, title: title});

		if (this.favs.length > 20) {
			this.favs.shift();
		}

		this.setFavs();
	}

	isFav(name: string) {
		return this.favs.some(el => el.name === name);
	}

	removeFromFav(name: string) {
		this.favs = this.favs.filter(el => el.name !== name);
		this.setFavs();
	}
}
