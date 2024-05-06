import {combineLatest, Observable, Subject, Subscription} from 'rxjs';
import {Connection_State, Scheme, Project} from '../user';
import {takeUntil} from 'rxjs/operators';
import {HttpClient} from '@angular/common/http';
import {TranslateService} from '@ngx-translate/core';
import {Injectable, OnDestroy} from '@angular/core';
import {DIG_Status_Category, DIG_Status_Type} from '../scheme/scheme';

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
    public schemes: Project[] = [];

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

    // protected getStatuses(schemes: Project[] = this.schemes) {
    //     schemes
    //         .filter(h => !!h)
    //         .map(h => {
    //             const id = h.parent || h.id;

    //             h.mod_state = false;
    //             h.loses_state = false;
    //             h.status_checked = false;
    //             h.connect_state = Connection_State.CS_SERVER_DOWN;

    //             // get status
    //             const sub = this.httpGet<StatusItems>(`/api/v2/scheme/${h.id}/dig_status`).subscribe(statusItems => {
    //                 h.messages = []; // 0 messages, wait

    //                 // set connection status
    //                 h.connection = statusItems.connection;
    //                 const [connState, modState, losesState] = SchemesList.parseConnectNumber(h.connection);
    //                 h.mod_state = <boolean>modState;
    //                 h.loses_state = <boolean>losesState;
    //                 h.status_checked = true;
    //                 h.connect_state = <Connection_State>connState;

    //                 // set messages
    //                 if (this.statusInfo[id]) { // if we have StatusInfo
    //                     // do it now
    //                     this.putMessages(h.id, statusItems, this.statusInfo[id], schemes);
    //                 } else { // if we haven't StatusInfo
    //                     // put into queue
    //                     if (!this.statusQueue[id]) {
    //                         this.statusQueue[id] = {isLoading: false, depSchemes: []}; // create a place in queue
    //                     }
    //                     this.statusQueue[id].depSchemes.push({id: h.id, si: statusItems}); // put scheme as a depeneded

    //                     if (!this.statusQueue[id].isLoading) {
    //                         // start loading if was not started
    //                         this.statusQueue[id].isLoading = true;
    //                         this.getStatusInfo(id, h.id, schemes);
    //                     }
    //                 }

    //                 sub.unsubscribe();
    //             });

    //             this.statusItemSubs.push(sub);
    //         });
    // }

    private getStatusInfo(id: number, real_id: number, schemes: Scheme[]) {
        const statusInfoSubs = combineLatest([
            this.http.get<DIG_Status_Type[]>(`/api/v2/scheme/${real_id}/dig_status_type`),
            this.http.get<DIG_Status_Category[]>(`/api/v2/scheme/${real_id}/structure/dig_status_category/`),
        ]).subscribe(([statusInfo, statusCategories]) => {
            this.statusInfo[id] = statusInfo.map((info) => ({
                ...info,
                category: statusCategories.find((cat) => cat.id === info.category_id),
            }));

            /*
            console.log(`${id} is loaded`);
            console.log(statusInfo);
             */

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
