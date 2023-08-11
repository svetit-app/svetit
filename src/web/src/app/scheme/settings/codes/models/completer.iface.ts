import { CompletionModel } from './completion.model';
import * as ace from 'brace';

export interface ICompleter {

  getCompletions(
    editor: ace.Editor,
    session: ace.IEditSession,
    pos: ace.Position,
    prefix: string,
    callback: (data: any | null, completions: CompletionModel[]) => void
  ): void;

  getDocTooltip(item: CompletionModel): void;
}


