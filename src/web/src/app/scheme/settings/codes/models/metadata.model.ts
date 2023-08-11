export interface MetaInfoModel {
  // название сущности
  Name: string;
  // описание
  Description: string;
  // возвращаемый тип значения
  Type: string;
  // список вложенных элементов
  Children: MetaInfoModel[];
  // входные параметры, если это метод
  InputParameters?: { [name: string]: string };
}


