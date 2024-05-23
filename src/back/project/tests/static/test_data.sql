INSERT INTO project.project (id, space_id, key, name, description, changed_at, sync) VALUES
('11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'project1', 'Project 1', 'Description of Project 1', NOW(), 'projectToNode');

INSERT INTO project.param_type (id, space_id, parent_id, key, name, description, value_type) VALUES
(1, '11111111-1111-1111-1111-111111111111', NULL, 'paramType1', 'Param Type 1', 'Description of Param Type 1', 'int'),
(2, '11111111-1111-1111-1111-111111111111', 1, 'paramType2', 'Param Type 2', 'Description of Param Type 2', 'int'),
(3, '11111111-1111-1111-1111-111111111111', 1, 'paramType3', 'Param Type 3', 'Description of Param Type 3', 'int'),
(4, '11111111-1111-1111-1111-111111111111', 1, 'paramType4', 'Param Type 4', 'Description of Param Type 4', 'int'),
(5, '11111111-1111-1111-1111-111111111111', 1, 'paramType5', 'Param Type 5', 'Description of Param Type 5', 'int'),
(6, '11111111-1111-1111-1111-111111111111', 1, 'paramType6', 'Param Type 6', 'Description of Param Type 6', 'int');

SELECT setval('project.param_type_id_seq', max(id)) FROM project.param_type;

INSERT INTO project.project_param (space_id, project_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1),
('11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 2);

INSERT INTO project.section (id, space_id, project_id, name) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'Section 1'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'Section 2');

SELECT setval('project.section_id_seq', max(id)) FROM project.section;

INSERT INTO project.section_param (space_id, section_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 2),
('11111111-1111-1111-1111-111111111111', 1, 3);

INSERT INTO project.cc_type (id, space_id, project_id, key, name, description) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ccType1', 'CC Type 1', 'Description of CC Type 1'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ccType2', 'CC Type 2', 'Description of CC Type 2');

SELECT setval('project.cc_type_id_seq', max(id)) FROM project.cc_type;

INSERT INTO project.control_circuit (id, space_id, type_id, section_id, name) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1, 1, 'Control Circuit 1'),
(2, '11111111-1111-1111-1111-111111111111', 1, 1, 'Control Circuit 2');

SELECT setval('project.control_circuit_id_seq', max(id)) FROM project.control_circuit;

INSERT INTO project.plugin (id, space_id, project_id, name, description, key) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'Plugin 1', 'Description of Plugin 1', 'plugin_1'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'Plugin 2', 'Description of Plugin 2', 'plugin_2');

SELECT setval('project.plugin_id_seq', max(id)) FROM project.plugin;

INSERT INTO project.device (id, space_id, project_id, plugin_id, name, check_interval_msec) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 'Device 1', 1000),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 'Device 2', 1000);

SELECT setval('project.device_id_seq', max(id)) FROM project.device;

INSERT INTO project.device_plugin_param (space_id, device_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 3),
('11111111-1111-1111-1111-111111111111', 1, 4);

INSERT INTO project.code (id, space_id, project_id, repository_id, commit_hash) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ff68dd7233213841e710a492c83cba8fe2380a88');

SELECT setval('project.code_id_seq', max(id)) FROM project.code;

INSERT INTO project.measure (id, space_id, project_id, name) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'M1'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'M2');

SELECT setval('project.measure_id_seq', max(id)) FROM project.measure;

INSERT INTO project.save_timer (id, space_id, project_id, interval_msec) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1000);

SELECT setval('project.save_timer_id_seq', max(id)) FROM project.save_timer;

INSERT INTO project.cc_type_param (space_id, cc_type_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 4),
('11111111-1111-1111-1111-111111111111', 1, 3);

INSERT INTO project.di_type (id, space_id, project_id, measure_id, save_timer_id, key, name, mode, save_algorithm) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 1, 'diType1', 'DI Type 1', 'readonlyFlag', 'off'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 1, 'diType2', 'DI Type 2', 'readonlyFlag', 'off');

SELECT setval('project.di_type_id_seq', max(id)) FROM project.di_type;

INSERT INTO project.di_plugin_param (space_id, di_type_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 5),
('11111111-1111-1111-1111-111111111111', 1, 4);

INSERT INTO project.cc_type_di_type (space_id, cc_type_id, di_type_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 1),
('11111111-1111-1111-1111-111111111111', 2, 1);

INSERT INTO project.device_item (id, space_id, device_id, type_id, name) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1, 1, 'Device Item 1'),
(2, '11111111-1111-1111-1111-111111111111', 1, 1, 'Device Item 2');

SELECT setval('project.device_item_id_seq', max(id)) FROM project.device_item;

INSERT INTO project.cc_mode_type (id, space_id, project_id, cc_type_id, key, name) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 'ccModeType1', 'CC Mode Type 1'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 1, 'ccModeType2', 'CC Mode Type 2');

SELECT setval('project.cc_mode_type_id_seq', max(id)) FROM project.cc_mode_type;

INSERT INTO project.cc_di (space_id, cc_id, di_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 1),
('11111111-1111-1111-1111-111111111111', 2, 1);

INSERT INTO project.cc_param (space_id, cc_id, param_id) VALUES
('11111111-1111-1111-1111-111111111111', 1, 6),
('11111111-1111-1111-1111-111111111111', 1, 2);

INSERT INTO project.cc_status_category (id, space_id, project_id, key, name, color) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ccStatusCategory1', 'CC Status Category 1', 'black'),
(2, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ccStatusCategory2', 'CC Status Category 2', 'black');

SELECT setval('project.cc_status_category_id_seq', max(id)) FROM project.cc_status_category;

INSERT INTO project.cc_status_type (id, space_id, cc_type_id, category_id, key, text, inform) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1, 1, 'ccStatusType1', 'CC Status Type 1', FALSE),
(2, '11111111-1111-1111-1111-111111111111', 1, 1, 'ccStatusType2', 'CC Status Type 2', FALSE);

SELECT setval('project.cc_status_type_id_seq', max(id)) FROM project.cc_status_type;

INSERT INTO project.value_view (id, space_id, di_type_id, value, view) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1, 'Value1', 'View1'),
(2, '11111111-1111-1111-1111-111111111111', 1, 'Value2', 'View2');

SELECT setval('project.value_view_id_seq', max(id)) FROM project.value_view;

INSERT INTO project.translation (id, space_id, project_id, lang, key, value) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ru', 'key1', 'value1');

SELECT setval('project.translation_id_seq', max(id)) FROM project.translation;