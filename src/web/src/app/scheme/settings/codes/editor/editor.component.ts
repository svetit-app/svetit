import {
  Component,
  OnInit,
  ViewChild,
  AfterViewInit,
  Input,
  Output,
  EventEmitter,
  OnDestroy
} from '@angular/core';

import * as ace from 'brace';
import 'brace/mode/javascript';
import 'brace/mode/typescript';
import '../dracula-mod';
import 'brace/ext/searchbox';
import 'brace/ext/language_tools';
import 'brace/snippets/javascript';

import { CompleterService } from '../services/completer.service';
import { MetaInfoModel } from '../models/metadata.model';

@Component({
  selector: 'app-editor',
  templateUrl: './editor.component.html',
  styleUrls: ['./editor.component.css']
})
export class EditorComponent implements OnInit, AfterViewInit, OnDestroy {
  @Input() metaInfo: MetaInfoModel[];
  script = '';
  options = {
    fontSize: '11pt',
    enableBasicAutocompletion: true,
    enableLiveAutocompletion: true,
    enableSnippets: true
  };

  @ViewChild('editor', { static: true })
  editor: any;

  @Output() textChanged = new EventEmitter<void>();

  constructor(private completer: CompleterService) {}

  ngOnInit() {
    this.completer.setMetadata(this.metaInfo);
  }

  ngAfterViewInit() {
    const langTools = ace.acequire('ace/ext/language_tools');
    langTools.setCompleters([this.completer]);
 
    this.editor.getEditor().setAutoScrollEditorIntoView(true);
 }

  ngOnDestroy(): void {
    // Вернем дефолтные комплетеры
    const langTools = ace.acequire('ace/ext/language_tools');
    const { textCompleter, keyWordCompleter, snippetCompleter } = langTools;
    langTools.setCompleters([
      textCompleter,
      keyWordCompleter,
      snippetCompleter
    ]);
  }

  getEditor(): ace.Editor {
    return this.editor._editor;
  }


  adjustSize() {
    const ed = this.editor.getEditor();
    ed.resize();
  }

  setText(text: string): void {
      this.editor.setText(text);
      this.editor._editor.session.setUndoManager(new ace.UndoManager());
  }

  onTextChanged(): void {
    this.textChanged.emit();
  }
}


