# DeviceAccess Angular Frontend

## Описание работы приложения
### Введение
Приложение обменивается структурами данных с сервером посредством REST API отправляя JSON рапросы к серверу. Описание структуры данных авторизации и списка проектов находятся в файле **src/app/user.ts**, а структуры данных проекта в файле **src/app/house/house.ts**. Для _POST_ запросов, должен передаваться _CSRF_ токен в заголовке **X-CSRFToken** получаемый из _Cookie_ **csrftoken**.

#### Список путей URL REST API и возвращаемые структуры данных:
##### Авторизация
- /api/auth/ -> User
- /api/refresh/
- /api/verify/

##### Список проектов
- /api/v1/house/ -> PaginatorApi\<House>
  
##### Работа с проектом
- /api/v1/detail/ -> HouseDetail
- /api/v1/events/ -> PaginatorApi\<EventLog>
- /api/v1/logs/ -> PaginatorApi\<Logs>
- /api/v1/code/ -> Codes

### Авторизация
Пользователь заходит на сайт и попадает на страницу авторизации (компонент страницы **src/app/login**). Приложение посылает логин и пароль на **/api/auth/** и в ответ получает структуру **User** c описанием данных пользователя, его разрешений и ключ сессии.

### Запрос списка проектов
Пользователь заходит на страницу _Панель (/src/app/houses/dashboard/)_ или _Проекты (/src/app/houses/list/)_. Приложение запрашивает список проектов пользователя посылая _GET_ запрос на **/api/v1/house/** с указанием параметров для **PaginatorApi** например: "?**limit**=10&**offset**=0&**ordering**=title" запрос 10 проектов начиная с 0 отсортировать по "title". Также пользователь может воспользоваться _Поиском (/src/app/houses/search/)_ приложение при этом передаёт параметр _?**search**=текст_.

### Выбор проекта
Пользователь нажимает на имя проекта с которым будет работать и попадает в модуль **House**(_/src/app/house/house.module.ts_) основной компонент которого **HouseComponent**(_/src/app/house/house.component.*_) содержит в себе меню для перехода к конкретным компонентам работы с проектом, а также закреплёную панель с выводом информации о состоянии подключения к проекту. 
#### Пункты меню:
- Сведения (_/src/app/houses/detail_)
- Обзор (_/src/app/house/view_)
- Управление (_/src/app/house/manage_)
- Журнал (_/src/app/house/log_)
- Cтруктура (_/src/app/house/settings_)
- Отчёты (_/src/app/house/reports_)

Компонент **HouseComponent** при инициализации запрашивает описание проекта с помощью службы **HouseService**(_/src/app/house/house.service.ts_) которая в свою очередь посылает _GET_ запрос на **/api/v1/detail/?id=10** где 10 это ID выбранного проекта. В ответ приложение получает структуру **HouseDetail** и обрабатывает её. 

### Установка постоянного соединения с сервером
В файле _/src/app/web-socket.service.ts_ описаны базовые классы для работы с WebSocket. Это класс **ByteTools** который содержит **статические** методы для обработки байтовый сообщений. Класс службы **WebSocketBytesService** обеспечивает базовое взаимодействие с WebSocket сервером и служит основой для основного класса службы **ControlService** описывающего протокол общения по WebSocket, который находится в файле _/src/app/house/control.service.ts_.

После выбора проекта, приложение подключается к _WebSocket_ и авторизуется передав ключ сессии полученный при авторизации на web сервере. На панеле при этом горит **серый** индикатор сообщающий о состоянии ожидания подключения. При неудачной установке соединения с сервером загорается **красный** индикатор. В случае успеха запрашивается состояние подключения выбранного проекта (**Cmd.ConnectInfo**) и если проект подключен загорается **зелёный** индикатор в противном случае **жёлтый**.

## Cтруктуры данных
Поля структур прокоментированы в файлах определения.

### Файл src/app/user.ts
Содержит следующие структуры: Шаблон массива данных - **PaginatorApi\<T>**, Пользователь - **User**, Проект - **House**

### Файл src/app/house/house.ts
Содержит следующие структуры: Описание проекта - **HouseDetail**, Устройство - **Device**, Элемент устройства - **DeviceItem**, Тип элемента - **ItemType**, Еденица измерения - **SignType**, Секция - **Section**, Группа - **Group**, Тип группы -  **GroupType**, Уставка - **ParamValue**, Тип уставки - **ParamItem**, и некоторые другие.

## Протокол общения по WebSocket
Приложение обменивается с WebSocket сервером сообщениями в виде массива байт. Стразу после установки соединения сервер посылает один байт **Cmd.Auth**(1) на что клиент отвечает ключом сессии. В случае успешной авторизации сервер возврашает один байт **Cmd.Welcome**(2) в противном случае разрывает соединение.
### Структура сообщения
Кроме Auth и Welcome структура следующая: 
[1 байт **CMD**][4 байта **PROJECT_ID**][N байт **ТЕЛО ЗАПРОСА**]

### Сообщения посылаемые клиентом
#### Cmd.Auth (1) - Авторизация. Ответ: Cmd.Welcome
[1][0][N TOKEN]

#### Cmd.ConnectInfo (3) - Подключён ли проект или нет. Ответ: Cmd.ConnectInfo
[3][PROJ_ID]

#### Cmd.WriteToDevItem (4) - Изменить значение элемента (Вкл./Выкл./Значение)
[4][PROJ_ID][4байта ITEM_ID][QVariant RAW_VALUE]

#### Cmd.ChangeGroupMode (5) - Изменить режим автоматизации
[5][PROJ_ID][4байта MODE_ID][4байта GROUP_ID]

#### Cmd.ChangeParamValues (6) - Изменить уставки
[6][PROJ_ID][QByteArray] - (Расписать содержимое QByteArray)

#### Cmd.ChangeCode (7) - Изменить скрипт автоматизации
[7][PROJ_ID][4байта CODE_ID][QString TEXT]

#### Cmd.ExecScript (8) - Выполнить JavaScript команду
[8][PROJ_ID][QString TEXT]

#### Cmd.Restart (9) - Перезагрузка
[8][PROJ_ID]

### Сообщения посылаемые сервером
#### Cmd.Auth (1) - Авторизация
[1]

#### Cmd.Welcome (2) - Авторизация успешна
[2]

#### Cmd.ConnectInfo (3) - Состояние подключения проекта
[3][PROJ_ID][1байт BOOLEAN Connected][QString IP][INT64 time milliseconds][QString TimeZone name]

#### Cmd.DevItemValues (10) - Новые значения элементов
[10][PROJ_ID][4байта PACK_SIZE][[4байта ITEM_ID][QVariant raw_value][QVariant display_value][Повторяется PACK_SIZE раз]

#### Cmd.Eventlog (11) - Новое сообщение в журнале
[11][PROJ_ID][4байта EVENT_ID][4байта TYPE_ID][QString ISODateWithMs][QString category][QString message]

#### Cmd.GroupMode (12) - Новое значение режима автоматизации
[12][PROJ_ID][4байта MODE_ID][4байта GROUP_ID]

---

This project was generated with [Angular CLI](https://github.com/angular/angular-cli) version 1.7.0.

## Development server

Run `ng serve` for a dev server. Navigate to `http://localhost:4200/`. The app will automatically reload if you change any of the source files.

## Code scaffolding

Run `ng generate component component-name` to generate a new component. You can also use `ng generate directive|pipe|service|class|guard|interface|enum|module`.

## Build

Run `ng build` to build the project. The build artifacts will be stored in the `dist/` directory. Use the `-prod` flag for a production build.

## Running unit tests

Run `ng test` to execute the unit tests via [Karma](https://karma-runner.github.io).

## Running end-to-end tests

Run `ng e2e` to execute the end-to-end tests via [Protractor](http://www.protractortest.org/).

## Further help

To get more help on the Angular CLI use `ng help` or go check out the [Angular CLI README](https://github.com/angular/angular-cli/blob/master/README.md).
-
