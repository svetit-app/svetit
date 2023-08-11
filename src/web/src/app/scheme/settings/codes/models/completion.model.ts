export interface CompletionModel {
  caption: string;
  description?: string;
  snippet?: string;
  meta: string;
  type: string;
  value?: string;
  parent?: string;
  docHTML?: string;
  // Входные параметры. Где ключ - имя параметры, значение - тип
  inputParameters?: { [name: string]: string };
}


