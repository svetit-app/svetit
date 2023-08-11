import {Component, OnInit} from '@angular/core';

import {SchemeService} from '../../scheme.service';
import {Save_Timer} from '../../scheme';

import {ChangeTemplate, Structure_Type} from '../settings';
import {SettingsService} from '../../settings.service';
import {UIService} from '../../../ui.service';

@Component({
  selector: 'app-save-timers',
  templateUrl: './save-timers.component.html',
  styleUrls: ['../settings.css', './save-timers.component.css']
})
export class SaveTimersComponent extends ChangeTemplate<Save_Timer> implements OnInit {
  constructor(
    schemeService: SchemeService,
    ui: UIService,
    private settingsService: SettingsService,
  ) {
    super(schemeService, Save_Timer, Structure_Type.ST_SAVE_TIMER, ui);
  }

  save_timers: Save_Timer[];
  getObjects(): Save_Timer[] {
    return this.save_timers;
  }

  ngOnInit() {
    this.settingsService.getSaveTimers().subscribe(api => {
      this.save_timers = api.results;
      this.fillItems();
    });
  }

  initItem(obj: Save_Timer): void
  {
    obj.interval = 1000;
  }
}
