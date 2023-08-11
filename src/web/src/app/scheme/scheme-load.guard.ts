import { Injectable } from '@angular/core';
import { CanActivate, CanActivateChild, ActivatedRouteSnapshot, RouterStateSnapshot } from '@angular/router';
import { Router, UrlTree, UrlSegmentGroup, PRIMARY_OUTLET } from '@angular/router';
import { Observable } from 'rxjs/Observable';
import 'rxjs/add/operator/map';

import { SchemeService } from './scheme.service';
import { AuthService } from '../auth/service';

@Injectable()
export class SchemeLoadGuard implements CanActivate, CanActivateChild {

  constructor(
    private router: Router,
    private auth: AuthService,
    private schemeService: SchemeService
  ) {}

  canActivate(
    next: ActivatedRouteSnapshot,
    state: RouterStateSnapshot): Observable<boolean> | Promise<boolean> | boolean {
    if (this.schemeService.scheme) {
      return true;
    }

    const name = next.paramMap.get('name');
    return this.schemeService.loadScheme2(name);
  }

  canActivateChild(next: ActivatedRouteSnapshot, state: RouterStateSnapshot): Observable<boolean> | Promise<boolean> | boolean {
    if (next.data['is_edit']) {
      const tree: UrlTree = this.router.parseUrl(state.url);
      const g: UrlSegmentGroup = tree.root.children[PRIMARY_OUTLET];
      const segments: any[] = [g.segments[0].path, g.segments[1].path, 'detail'];
      this.router.navigate(segments);
      return false;
    }

    // const id = +next.paramMap.get('id');
    // console.log(`load guard start child: ${state.url} scheme_id: ${id}`);
    return true;
/*
    return Observable.timer(5000).map(() => {
      console.log('load guard timeout');
      return true;
    });*/

//    return this.canActivate(route, state);
  }
}
