import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {MediaMatcher} from '@angular/cdk/layout';
import {TranslateService} from '@ngx-translate/core';
import {UIService} from '../ui.service';
import {AuthService} from '../auth/service';

interface NavLink {
  link: string;
  text: string;
  icon: string;
}

@Component({
  selector: 'app-user-settings',
  templateUrl: './user-settings.component.html',
  styleUrls: ['./user-settings.component.css']
})
export class UserSettingsComponent implements OnInit {
  constructor(
    public translate: TranslateService,
    public auth: AuthService,
  ) {

  }

  ngOnInit() {
  }

}
