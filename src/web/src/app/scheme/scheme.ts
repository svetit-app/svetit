import {Observable} from 'rxjs';
import {Hsl} from './reports/charts/color-picker-dialog/color-picker-dialog';

export class Code_Item { // Скрипт автоматизации
  id: number;   // ID
  name: string; // Имя
  text: string; // Скрипт
  global_id: number; // Global id
}

export class DIG_Type { // Тип группы
  id: number;           // ID
  name: string;         // Имя
  title: string;        // Заголовок
  description: string;
}

export class Sign_Type { // Еденица измерения
  id: number;   // ID
  name: string; // Еденица измерения
}

export class Plugin_Type { // Плагин работы с устройством
  id: number;   // ID
  name: string; // Имя плагина
  param_names_device: string; // "is_number|is_little_endian"
  param_names_device_item: string; // "data"
}

export class Mnemoscheme {
    id: number;
    title: string;
}

export enum Register_Type { // Тип данных элемента
    RT_INVALID,
    RT_DISCRETE_INPUTS,   // Булевы значения только для чтения
    RT_COILS,             // Булевы значения чтение/запись
    RT_INPUT_REGISTERS,   // Значения только для чтения
    RT_HOLDING_REGISTERS, // Значения чтение/запись
    RT_FILE,              // Передача файла
    RT_SIMPLE_BUTTON,     // Обычная кнопка. Передает всегда значение 1
    RT_VIDEO_STREAM,      // Видео поток
}

export enum Save_Algorithm { // Алгоритм сохранения изменений значения
    SA_INVALID,
    SA_OFF,               // Не сохранять автоматически
    SA_IMMEDIATELY,       // Сохранять немедленно
    SA_BY_TIMER,          // Сохранять по таймеру
    SA_BY_TIMER_ANY_CASE, // Сохранять по таймеру даже если значение не изменилось
}

export class Device_Item_Type { // Тип элемента
  id: number;               // ID
  name: string;             // Имя элемента для использования в скриптах автоматизации
  title: string;            // Отображаемое имя элемента по-умолчанию
  group_type_id: number;     // ID типа группы к которой может принадлежать элемент (Плохой?)
  sign_id: number;          // ID еденицы измерения
  sign: Sign_Type;           // Еденица измерения
  register_type: Register_Type;     // Тип данных элемента
  save_algorithm: Save_Algorithm;    // Алгоритм сохранения изменений значения
  save_timer_id: number;    // ID таймера если выбран такой алгоритм сохранения

    views: Value_View[];
}

export class Section {
  id: number;       // ID
  name: string;     // Имя секции
  day_start: number; // Время начала дня в секундах
  day_end: number;   // Время конца для в секундах
  groups: Device_Item_Group[];  // Группы
}

export class Device_Item_Value {
  raw_value: any;      // Сырое значение
  value: any;  // Отображаемое значение
}

export class Device_Item { // Элемент устройства
  id: number;         // ID
  parent_id: number;  // ID родителя
  device_id: number;  // ID устройства
  group_id: number;   // ID группы

  name: string;       // Имя
  type_id: number;    // ID типа
  type: Device_Item_Type;     // Тип
  extra: string;      // Пользовательские параметры

  val: Device_Item_Value;
}

export class DIG_Status_Category {
  id: number;     // ID
  name: string;
  title: string;
  color: string;
}

export class DIG_Status_Type {
  id: number;           // ID
  group_type_id: number;
  category_id: number;
  category: DIG_Status_Category;
  name: string;
  text: string;
  inform: boolean;
}

export class DIG_Status {
  status: DIG_Status_Type;
  status_id: number;
  args: string[];
}

export class DIG_Status_Info {
  color: string;
  text: string;
  short_text: string;
}

export class Device_Item_Group {  // Группа
  id: number;               // ID
  title: string;
  section_id: number;       // ID секции
  type_id: number;          // ID типа группы
  mode: number;          // ID режима автоматизации
  type: DIG_Type;          // Тип группы
  items: Device_Item[] = []; // Элементы в группе
  params: DIG_Param[] = [];// Уставки
  status: number;           // Состояние
  statuses: DIG_Status[] = [];
  status_info: DIG_Status_Info;
}

export class Save_Timer {
  id: number;
  interval: number;
}

export enum DIG_Param_Value_Type
{
    VT_UNKNOWN = 0,
    VT_INT,
    VT_BOOL,
    VT_FLOAT,
    VT_STRING,
    VT_BYTES,
    VT_TIME,
    VT_RANGE, // Используется для задания двух дочерних элементов
    VT_COMBO, // Используется для задания дочерних элементов (Кажется не реализованно)
}

export class DIG_Param_Type {  // Тип уставки
  id: number;               // ID
  group_type_id: number;     // ID типа группы к которой может принадлежать уставка
  parent_id: number;        // ID родителя
  childs: DIG_Param_Type[] = []; // Дочерние типы

  title: string;                      // Отображаемое имя
  name: string;                       // Имя латиницей используется в скриптах
  description: string;                // Описание
  value_type: DIG_Param_Value_Type;   // Тип значения уставки
}

export class DIG_Param { // Уставка
  id: number;       // ID
  group_id: number; // ID группы
  param_id: number; // ID типа уставки
  value: string;    // Значение
  param: DIG_Param_Type; // Тип уставки
  childs: DIG_Param[] = [];
}

export class Device { // Устройство
  id: number;         // ID
  name: string;       // Имя
  plugin_id: number; // ID используемого плагина
  check_interval: number; // Интервал запуска проверки плагина
  extra: string;         // Пользовательские параметры
  items: Device_Item[];// Массив элементов
}

export class Log_Base {
//  id: number;     // ID
  timestamp_msecs: string | number;     // Время изменения // TODO: проверить где используется как string и устранить
  user_id: number;  // Пользователь
}

export class Log_Mode extends Log_Base {
    group_id: number;
    mode_id: number;
}

export class Log_Value extends Log_Base { // Запись журнала изменений значения
  item_id: number;  // ID элемент
  raw_value: string;// Значение
  value: string;    // Отображаемое значение
}

export enum Log_Event_Type { // Тип события в журнале событий
    ET_DEBUG = 0,
    ET_WARNING,
    ET_CRITICAL,
    ET_FATAL,
    ET_INFO,
    ET_USER
}

export class Log_Event extends Log_Base { // Запись в журнале событий
  date: Date;
  category: string;        // Категория
  text: string;        // Сообщение
  type_id: number;       // Тип события
  color: string = ''; // Цвет?
}

export enum Log_Status_Direction {
    SD_ADD = 1,
    SD_DEL = 2,
}

export class Log_Status extends Log_Base {
    group_id: number;
    status_id: number;
    args: string[];
    direction: Log_Status_Direction;
}

export class Log_Param extends Log_Base {
  group_param_id: number;
  value: string;
}

export class Settings {
  id: number;     // ID
  param: string;
  value: string;
}

export class DIG_Mode_Type {
  id: number;
  name: string;
  title: string;
  group_type_id: number;
}

export interface Axis_Config {
    id: string;

    isRight: boolean;
    display: false | 'auto';
    displayGrid: boolean;
    from: number | string;
    to: number | string;
    order: number | string;

    showModal?: boolean;
    allowDragOver?: boolean;
}

export class Chart_Item_Config {
    item_id: number;
    param_id: number;

    isParam: boolean;

    extra?: {
        hidden: boolean;
        stepped: boolean;
        color: Hsl;
    };
}

export class Chart_Item<T = Device_Item | DIG_Param> extends Chart_Item_Config {
    item: T;

    extra?: Chart_Item_Config['extra'] & {
        title: string;
        displayColor: string;
    };
}

export interface Axis_Params extends Axis_Config {
    datasets: Chart_Item[];
}

export class Chart_Item_old {
    item_id: number;
    param_id: number;

    extra?: {
        axis_params?: Omit<Axis_Config, "id"> & { stepped: boolean };
        color: string;
    };
}

export class Chart_old {
    id: number;
    name: string;
    items: Chart_Item_old[];
}

export class Chart_Base {
    id: number;
    name: string;
}

export class Chart extends Chart_Base {
    axes:  Axis_Params[];
}

export class Saved_User_Chart extends Chart_Base {
    axes: (Axis_Config & { datasets: Chart_Item_Config[] })[];
}

export class Disabled_Status {
    id: number;
    group_id: number; // Auth_Group
    dig_id: number;
    status_id: number;
    scheme_id: number;
}

export class Help {
    id: number;
    name: string;
    text: string;
    parent_id: number;
    scheme_id: number;
}

export class Translation {
    id: number;
    data: string;
    lang: string;
    scheme_id: number;
}

export interface Value_View {
    type_id: number;
    value: any;
    view: any;
}

export class Value_View_Detail {
    id: number;
    scheme_id: number;
    type_id: number;
    value: any;
    view: any;
}

export class Time_Info {
    utc_time: number; // Millisoconds
    tz_offset: number; // Seconds
    tz_name: string;
}

export class Scheme_Detail {
    id: number;             // ID проекта
    parent_id: number;
    name: string;           // Имя проекта
    title: string;

    sign_type: Sign_Type[];  // Еденицы измерения
    section: Section[];    // Наполнение секций
    device: Device[];      // Устройства и их элементы в системе
    device_item_type: Device_Item_Type[];  // Типы элементов
    dig_param_type: DIG_Param_Type[];    // Типы уставок
    dig_type: DIG_Type[];// Типы групп
    dig_status_type: DIG_Status_Type[];  // Типы состояний
    dig_status_category: DIG_Status_Category[];  // Категории состояний
    dig_mode_type: DIG_Mode_Type[];
    disabled_param: number[]; // Недоступные для пользователя параметры
    value_view: Value_View[];

    conn: Observable<number>;
}
