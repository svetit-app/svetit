import { Injectable } from '@angular/core';
import { ICompleter } from './../models/completer.iface';
import { CompletionModel } from './../models/completion.model';
import { MetaInfoModel } from './../models/metadata.model';

import * as ace from 'brace';
import 'brace/ext/language_tools';

const convertToCompletion = (data: MetaInfoModel, parent?: string) => {
  const isMethod = data.hasOwnProperty('InputParameters');
  const caption = data.Name;
  const type = data.Type;
  let meta: string;
  if (isMethod) {
    meta = 'method';
  } else {
    meta = parent ? 'property' : 'class';
  }

  const result: CompletionModel = {
    caption,
    meta,
    type,
    parent,
    description: data.Description,
    inputParameters: isMethod ? data.InputParameters : {}
  };
  let snippet = '';
  if (isMethod) {
    const signature = Object.keys(data.InputParameters).map((key, index) => {
      const value = data.InputParameters[key];
      return '${' + (index + 1) + ':' + value + '}';
    });
    snippet = `${data.Name}(${signature.join(', ')})`;
  }
  if (snippet) {
    result.snippet = snippet;
    result.value = snippet;
  } else {
    result.value = caption;
  }
  return result;
};

@Injectable()
export class CompleterService implements ICompleter {
  private completions: { [name: string]: CompletionModel[] } = {};
  private completionsTree: { [name: string]: string[] } = {};
  private roots: string[] = [];

  private snippets: CompletionModel[] = [];
  private keywords: CompletionModel[] = [];

  private snippetManager = ace.acequire('ace/snippets').snippetManager;
  private textCompleter = ace.acequire('ace/autocomplete/text_completer');
  constructor() {}

  setMetadata(meta: MetaInfoModel[]): void {
    const register = (name: string, item: MetaInfoModel, parent?: string) => {
      if (!this.completions[name]) {
        this.completions[name] = [];
      }
      this.completions[name].push(convertToCompletion(item, parent));
    };
    const getItems = (item: MetaInfoModel, parentId: string = null) => {
      register(item.Name, item, parentId);
      const children = item.Children || [];
      this.completionsTree[item.Name] = children.map(x => x.Name);

      const methods = children.filter(x => x.hasOwnProperty('InputParameters'));
      const props = children.filter(x => !x.hasOwnProperty('InputParameters'));

      for (const child of methods) {
        register(child.Name, child, item.Name);
      }

      for (const child of props) {
        getItems(child, item.Name);
      }
    };

    for (const field of meta) {
      this.roots.push(field.Name);
      getItems(field);
      
    }
  }

  getCompletions(
    editor: ace.Editor,
    session: ace.IEditSession,
    pos: ace.Position,
    prefix: string,
    callback: (data: any | null, completions: CompletionModel[]) => void
  ): void {
    const completions: CompletionModel[] = [];
    const parentId = this._getParentId(
      session.getLine(pos.row),
      pos.column,
      prefix
    );

    const fillText = () => {
      this.textCompleter.getCompletions(
        editor,
        session,
        pos,
        prefix,
        (err, result) => {
          if (result && result.length > 0) {
            completions.push(...result);
          }
        }
      );
    };
    if (!parentId) {
      this._fillSnippets(editor);
      this._fillKeywords(editor, session, pos, prefix);
      completions.push(...this._getRoots());
      completions.push(...this.snippets, ...this.keywords);
      fillText();
    } else {
      const children = this._getChildren(parentId);
      completions.push(...children);

      if (children.length === 0) {
        fillText();
      }
    }

    callback(null, completions);
  }

  getDocTooltip(item: CompletionModel): void {
    if (item.type === 'snippet') {
      if (item.type === 'snippet' && !item.docHTML) {
        item.docHTML = [
          '<b>',
          item.caption,
          '</b>',
          '<hr></hr>',
          item.snippet
        ].join('');
      }
    }
    if (item.inputParameters) {
      let signature = Object.keys(item.inputParameters)
        .map(x => `${x} ${item.inputParameters[x]}`)
        .join(', ');

      if (signature) {
        signature = `(${signature})`;
      }

      if (!item.docHTML) {
        const path = [];
        if (item.parent) {
          let parentId = item.parent;
          while (parentId) {
            path.push(parentId);
            parentId = this.completions[parentId][0].parent;
          }
        }
        const displayName =
          [...path.reverse(), item.caption].join('.') + signature;
        let type = item.type;
        if (item.meta === 'class') {
          type = 'class';
        } else if (item.type.toLowerCase() === 'void') {
          type = 'void';
        }

        const description = item.description || '';
        let html = `<b>${type} ${displayName}</b><hr>`;
        if (description) {
          html += `<p style="max-width: 400px; white-space: normal;">${description}</p>`;
        }
        item.docHTML = html;
      }
    }
  }

  private _fillSnippets(editor) {
    if (this.snippets.length > 0) {
      return;
    }
    const snippetMap = this.snippetManager.snippetMap;
    for (const scope of this.snippetManager.getActiveScopes(editor)) {
      const snippets = snippetMap[scope] || [];
      for (let i = snippets.length; i--; ) {
        const s = snippets[i];
        const caption = s.name || s.tabTrigger;
        if (!caption) {
          continue;
        }
        this.snippets.push({
          caption,
          snippet: s.content,
          meta: s.tabTrigger && !s.name ? s.tabTrigger + '\u21E5 ' : 'snippet',
          type: 'snippet'
        });
      }
    }
  }

  private _fillKeywords(
    editor: ace.Editor,
    session: ace.IEditSession,
    pos: ace.Position,
    prefix: string
  ) {
    const callback = (err, result) => {
      if (result && result.length && this.keywords.length === 0) {
        this.keywords.push(...result);
      }
    };
    if ((session.$mode as any).completer) {
      return (session.$mode as any).completer.getCompletions(
        editor,
        session,
        pos,
        prefix,
        callback
      );
    }
    const state = editor.session.getState(pos.row);
    const completions = (session.$mode as any).getCompletions(
      state,
      session,
      pos,
      prefix
    );
    callback(null, completions);
  }

  private _getChildren(parentId: string): CompletionModel[] {
    const children = this.completionsTree[parentId] || [];
    const result = [];
    for (const child of children) {
      const item = this.completions[child].find(x => x.parent === parentId);
      if (item) {
        result.push(item);
      }
    }
    return result;
  }

  private _getParentId(line: string, column: number, prefix: string): string {
    const reversed = Array.from(line)
      .reverse()
      .join('');
    const newPos = line.length - (column - prefix.length);

    const sub = reversed.substring(newPos);
    if (!sub.startsWith('.')) {
      return '';
    }

    const splitted = sub.substring(1).split('.');
    if (splitted.length > 0) {
      let value = Array.from(splitted[0])
        .reverse()
        .join('');
      ['(', '[', ' '].forEach(token => {
        const index = value.lastIndexOf(token);
        if (index > -1) {
          value = value.substring(index + 1);
        }
      });
      return value;
    } else {
      return '';
    }
  }

  private _getRoots(): CompletionModel[] {
    return this.roots.map(x => this.completions[x][0]);
  }

  getSnippet(item: string) {
    const parsed = item.split('.');
    if (parsed.length === 1) {
      return parsed[0];
    }
    const [parentId, itemId] = parsed;
    const children = this._getChildren(parentId);
    const found = children.find(
      x => x.caption.toLocaleLowerCase() === itemId.toLowerCase()
    );
    return found ? found.snippet : null;
  }
}

