import {Component, OnDestroy, OnInit} from '@angular/core';
import {HttpClient} from '@angular/common/http';

import {Subject} from 'rxjs';
import {debounceTime, distinctUntilChanged} from 'rxjs/operators';

import {Scheme} from '../../user';
import {SchemesService} from '../schemes.service';

@Component({
  selector: 'app-scheme-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.css']
})
export class SchemeSearchComponent implements OnInit, OnDestroy {
  private searchTerms = new Subject<string>();

  schemes: Scheme[] = [];

  constructor(private schemesService: SchemesService,
              protected http: HttpClient) {
  }

  // Push a search term into the observable stream.
  search(term: string): void {
    this.searchTerms.next(term);
  }

  recursiveSearch(query: string, next?: string) {
    this.schemesService.searchSchemes(query, next).subscribe((resp) => {
      this.schemes = this.schemes.concat(resp.results); // append schemes

      if (resp.next) { // if has next
        const nextUrl = new URL(resp.next);
        const nextQuery = nextUrl.search; // ?search=[QUERY]&limit=35&offset=[XX]

        this.recursiveSearch(query, nextQuery);
      }
    });
  }

  ngOnInit() {
    this.searchTerms.pipe(
      // wait 300ms after each keystroke before considering the term
      debounceTime(300),

      // ignore new term if same as previous term
      distinctUntilChanged(),
    ).subscribe((term) => {
      this.schemes = []; // new search!
      this.recursiveSearch(term);
    });
  }

  ngOnDestroy() {
    this.searchTerms.unsubscribe();
  }
}
