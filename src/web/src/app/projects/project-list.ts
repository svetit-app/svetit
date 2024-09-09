import {combineLatest, Observable, Subject, Subscription} from 'rxjs';
import {Connection_State, Scheme, Project} from '../user';
import {takeUntil} from 'rxjs/operators';
import {HttpClient} from '@angular/common/http';
import {TranslateService} from '@ngx-translate/core';
import {Injectable, OnDestroy} from '@angular/core';
import {DIG_Status_Category, DIG_Status_Type} from '../scheme/scheme';
import { Project as ApiProject } from '../api';

export class StatusItems {
	connection: number;
	items: {
		args: any;
		group_id: number;
		id: number;
		status_id: number;
		title: string;
	}[];
}

@Injectable()
export abstract class ProjectList implements OnDestroy {
	public schemes: ApiProject[] = [];

	private statusInfo: Record<number, DIG_Status_Type[]> = {};
	private statusQueue = {};
	protected statusItemSubs: Subscription[] = [];

	private httpReqs: Subject<void> = new Subject<void>();

	protected constructor(private http: HttpClient, private translate: TranslateService) {
	}

	ngOnDestroy(): void {
		// This aborts all HTTP requests.
		this.httpReqs.next();
		// This completes the subject properlly.
		this.httpReqs.complete();
	}

	private httpGet<T>(req: string): Observable<T> {
		return this.http.get<T>(req)
			.pipe( takeUntil(this.httpReqs) );
	}

	private getStatusInfo(id: number, real_id: number, schemes: Scheme[]) {
		const statusInfoSubs = combineLatest([
			this.http.get<DIG_Status_Type[]>(`/api/v2/scheme/${real_id}/dig_status_type`),
			this.http.get<DIG_Status_Category[]>(`/api/v2/scheme/${real_id}/structure/dig_status_category/`),
		]).subscribe(([statusInfo, statusCategories]) => {
			this.statusInfo[id] = statusInfo.map((info) => ({
				...info,
				category: statusCategories.find((cat) => cat.id === info.category_id),
			}));

			if (this.statusQueue[id]) {
				// parse a queue

				this.statusQueue[id].depSchemes.forEach((dh) => {
					this.putMessages(dh.id, dh.si, this.statusInfo[id], schemes);
				});
			}

			statusInfoSubs.unsubscribe();
		});
	}

	private putMessages(id: number, statusItems: StatusItems, st: DIG_Status_Type[], schemes?: Scheme[]) {
		const scheme = schemes.find(h => h.id === id);

		for (let i = 0; i < statusItems.items.length; i++) {
			const si = statusItems.items[i];

			const statusType = st.find(sti => sti.id === si.status_id);
			if (statusType) {
				let status;
				switch (statusType.category.name) {
					case 'Warn':
						status = 3;
						break;
					case 'Error':
						status = 4;
						break;
					case 'Ok':
						status = 1;
						break;
					case 'Undefined':
					default:
						status = 2;
				}
				scheme.messages.push({status, text: statusType.text, where: si.title});
			}
		}
	}

	private static parseConnectNumber(n: number) {
		// tslint:disable:no-bitwise
		const connState = n & ~Connection_State.CS_CONNECTED_MODIFIED & ~Connection_State.CS_CONNECTED_WITH_LOSSES;
		const modState = (n & Connection_State.CS_CONNECTED_MODIFIED) === Connection_State.CS_CONNECTED_MODIFIED;
		const losesState = (n & Connection_State.CS_CONNECTED_WITH_LOSSES) === Connection_State.CS_CONNECTED_WITH_LOSSES;
		// tslint:enable:no-bitwise

		return [connState, modState, losesState];
	}
}
