INSERT INTO project.project (id, space_id, key, name, description, changed_at, sync) VALUES
('11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'project1', 'Project 1', 'Description of Project 1', NOW(), 'project_to_node');

INSERT INTO project.param_type (id, parent_id, key, name, description, value_type, is_deleted) VALUES
(1, NULL, 'paramType1', 'Param Type 1', 'Description of Param Type 1', 'int', FALSE),
(2, NULL, 'paramType2', 'Param Type 2', 'Description of Param Type 2', 'int', FALSE),
(3, NULL, 'paramType3', 'Param Type 3', 'Description of Param Type 3', 'int', FALSE),
(4, NULL, 'paramType4', 'Param Type 4', 'Description of Param Type 4', 'int', FALSE),
(5, NULL, 'paramType5', 'Param Type 5', 'Description of Param Type 5', 'int', FALSE),
(6, NULL, 'paramType6', 'Param Type 6', 'Description of Param Type 6', 'int', FALSE);

SELECT setval('project.param_type_id_seq', max(id)) FROM project.param_type;

INSERT INTO project.project_param (project_id, param_id, is_deleted) VALUES
('11111111-1111-1111-1111-111111111111', 1, FALSE),
('11111111-1111-1111-1111-111111111111', 2, TRUE);

INSERT INTO project.section (id, project_id, name, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'Section 1', FALSE);

SELECT setval('project.section_id_seq', max(id)) FROM project.section;

INSERT INTO project.section_param (section_id, param_id, is_deleted) VALUES
(1, 2, FALSE);

INSERT INTO project.cc_type (id, project_id, key, name, description, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'ccType1', 'CC Type 1', 'Description of CC Type 1', FALSE);

SELECT setval('project.cc_type_id_seq', max(id)) FROM project.cc_type;

INSERT INTO project.control_circuit (id, type_id, section_id, name, is_deleted) VALUES
(1, 1, 1, 'Control Circuit 1', FALSE);

SELECT setval('project.control_circuit_id_seq', max(id)) FROM project.control_circuit;

INSERT INTO project.plugin (id, project_id, name, description, key, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'Plugin 1', 'Description of Plugin 1', 'plugin_1', FALSE);

SELECT setval('project.plugin_id_seq', max(id)) FROM project.plugin;

INSERT INTO project.device (id, project_id, plugin_id, name, check_interval_msec, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1, 'Device 1', 1000, FALSE);

SELECT setval('project.device_id_seq', max(id)) FROM project.device;

INSERT INTO project.device_plugin_param (device_id, param_id, is_deleted) VALUES
(1, 3, FALSE);

INSERT INTO project.code (id, project_id, repository_id, commit_hash) VALUES
(1, '11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'ff68dd7233213841e710a492c83cba8fe2380a88');

SELECT setval('project.code_id_seq', max(id)) FROM project.code;

INSERT INTO project.measure (id, project_id, name, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'M1', FALSE);

SELECT setval('project.measure_id_seq', max(id)) FROM project.measure;

INSERT INTO project.save_timer (id, project_id, interval_msec) VALUES
(1, '11111111-1111-1111-1111-111111111111', 1000);

SELECT setval('project.save_timer_id_seq', max(id)) FROM project.save_timer;

INSERT INTO project.cc_type_param (cc_type_id, param_id, is_deleted) VALUES
(1, 4, FALSE);

INSERT INTO project.di_type (id, measure_id, save_timer_id, key, name, mode, save_algorithm, is_deleted) VALUES
(1, 1, 1, 'diType1', 'DI Type 1', 'readonly_flag', 'off', FALSE),
(2, 1, 1, 'diType2', 'DI Type 2', 'readonly_flag', 'off', FALSE);

SELECT setval('project.di_type_id_seq', max(id)) FROM project.di_type;

INSERT INTO project.di_plugin_param (di_type_id, param_id, is_deleted) VALUES
(1, 5, FALSE);

INSERT INTO project.cc_type_di_type (cc_type_id, di_type_id, is_deleted) VALUES
(1, 1, FALSE);

INSERT INTO project.device_item (id, device_id, type_id, name, is_deleted) VALUES
(1, 1, 1, 'Device Item 1', FALSE),
(2, 1, 1, 'Device Item 2', FALSE);

SELECT setval('project.device_item_id_seq', max(id)) FROM project.device_item;

INSERT INTO project.cc_mode_type (id, cc_type_id, key, name, is_deleted) VALUES
(1, 1, 'ccModeType1', 'CC Mode Type 1', FALSE);

SELECT setval('project.cc_mode_type_id_seq', max(id)) FROM project.cc_mode_type;

INSERT INTO project.cc_di (cc_id, di_id, is_deleted) VALUES
(1, 1, FALSE);

INSERT INTO project.cc_param (cc_id, param_id, is_deleted) VALUES
(1, 6, FALSE);

INSERT INTO project.cc_status_category (id, project_id, key, name, color, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'ccStatusCategory1', 'CC Status Category 1', 'black', FALSE);

SELECT setval('project.cc_status_category_id_seq', max(id)) FROM project.cc_status_category;

INSERT INTO project.cc_status_type (id, cc_type_id, category_id, key, text, inform, is_deleted) VALUES
(1, 1, 1, 'ccStatusType1', 'CC Status Type 1', FALSE, FALSE);

SELECT setval('project.cc_status_type_id_seq', max(id)) FROM project.cc_status_type;

INSERT INTO project.value_view (id, di_type_id, value, view, is_deleted) VALUES
(1, 1, 'Value1', 'View1', FALSE);

SELECT setval('project.value_view_id_seq', max(id)) FROM project.value_view;

INSERT INTO project.translation (id, project_id, lang, key, value) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'ru', 'key1', 'value1');

SELECT setval('project.translation_id_seq', max(id)) FROM project.translation;