import {Component, OnInit} from '@angular/core';

import {SchemeService} from '../../scheme.service';
import {Plugin_Type} from '../../scheme';

import {ChangeTemplate, Structure_Type} from '../settings';
import {SettingsService} from '../../settings.service';
import {UIService} from '../../../ui.service';

@Component({
  selector: 'app-plugin-types',
  templateUrl: './plugin-types.component.html',
  styleUrls: ['../settings.css', './plugin-types.component.css']
})
export class PluginTypesComponent extends ChangeTemplate<Plugin_Type> implements OnInit {
  checkers: Plugin_Type[];

  constructor(
    schemeService: SchemeService,
    private settingsService: SettingsService,
    ui: UIService,
  ) {
    super(schemeService, Plugin_Type, Structure_Type.ST_PLUGIN_TYPE, ui);
  }

  getObjects(): Plugin_Type[] {
    return this.checkers;
  }

  ngOnInit() {
    this.settingsService.getPluginTypes().subscribe(ret => {
      this.checkers = ret.results;
      this.fillItems();
    });
  }
}
