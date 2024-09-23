import { ChangeDetectorRef, Component, OnInit, inject } from '@angular/core';
import {MediaMatcher} from '@angular/cdk/layout';
import {TranslateService} from '@ngx-translate/core';
import {UIService} from '../ui.service';
import {AuthService} from '../auth/service';
import { RouterModule } from '@angular/router';

interface NavLink {
  link: string;
  text: string;
  icon: string;
}

@Component({
	selector: 'app-user-settings',
	templateUrl: './user-settings.component.html',
	styleUrls: ['./user-settings.component.css'],
	standalone: true,
	imports: [RouterModule]
})
export class UserSettingsComponent {
}
