import {Component, OnInit, ViewChild} from '@angular/core';

import {SchemeService} from '../../scheme.service';
import {Code_Item} from '../../scheme';

import {ChangeInfo, ChangeTemplate, Structure_Type} from '../settings';
import {SettingsService} from '../../settings.service';

import {MetadataService} from './services/metadata.service';
import {UIService} from '../../../ui.service';
import {Observable} from 'rxjs/Observable';

@Component({
  selector: 'app-codes',
  templateUrl: './codes.component.html',
  styleUrls: ['../settings.css'/*, '../../../../assets/anonymous/stylesheet.css'*/, './codes.component.css']
})
export class Code_Item_Component extends ChangeTemplate<Code_Item> implements OnInit {
  metadata$ = this.metadataService.getMetadata();

  @ViewChild('codeEditor', {static: true}) editor;
  private newOpened = false;

  constructor(
    schemeService: SchemeService,
    private settingsService: SettingsService,
    private metadataService: MetadataService,
    ui: UIService,
  ) {
    super(schemeService, Code_Item, Structure_Type.ST_CODE_ITEM, ui);
  }

  getObjects(): Observable<Code_Item[]> {
      return this.settingsService.getCodes();
  }

  ngOnInit() {
      this.fillItems();
  }

  initItem(obj: Code_Item): void {
    obj.name = '';
  }

  name(obj: Code_Item): string {
    if (obj.name.length) {
      return obj.name;
    }
    return (<any>obj).d_name !== undefined ? (<any>obj).d_name : '<Empty>';
  }

  code_select(item: ChangeInfo<Code_Item>): void {
    if (this.sel_item !== item && !item.obj.text) {
      this.getCode(item);
    } else {
      this.select(item);
    }
  }

  select(item: ChangeInfo<Code_Item>): void {
    super.select(item);

    if (this.sel_item?.obj.text) {
      this.editor.setText(this.sel_item.obj.text);
      this.newOpened = true;
    }
  }

  getCode(code: ChangeInfo<Code_Item>): void {
    this.settingsService.getCode(code.obj.id).subscribe(full_code => {
      const state = code.state;
      code.obj.text = full_code.text;
      this.select(code);
      code.state = state;
    });
  }

  code_save(evnt): void {
    this.save(evnt);
    const elem = document.getElementById('editor-pane');
    elem.classList.remove('editor-pane-fullscreen');

    this.editor.adjustSize();
    setTimeout(() => { this.editor.adjustSize(); }, 200);
  }

  fullscreenToggle() {
    const elem = document.getElementById('editor-pane');
    elem.classList.toggle('editor-pane-fullscreen');

    this.editor.adjustSize();
    setTimeout(() => { this.editor.adjustSize(); }, 200);

    /*
    if (elem.requestFullscreen) {
      elem.requestFullscreen();
    } else if (elem.mozRequestFullScreen) {
      elem.mozRequestFullScreen();
    } else if (elem.webkitRequestFullscreen) {
      elem.webkitRequestFullscreen();
    } else if (elem.msRequestFullscreen) {
      elem.msRequestFullscreen();
    }
    */
  }

  wasChanged() {
    if (this.newOpened) {
      this.newOpened = false;
      return;
    }

    this.sel_item.obj.text = this.editor.script;
    this.itemChanged();
  }
}
