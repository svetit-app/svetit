INSERT INTO project.project (id, space_id, key, name, description, changed_at, sync) VALUES
('11111111-1111-1111-1111-111111111111', '11111111-1111-1111-1111-111111111111', 'project1', 'Project 1', 'Description of Project 1', NOW(), 'project_to_node');

INSERT INTO project.param_type (id, parent_id, key, name, description, value_type, is_deleted) VALUES
(1, NULL, 'paramType1', 'Param Type 1', 'Description of Param Type 1', 'int', FALSE),
(2, NULL, 'paramType2', 'Param Type 2', 'Description of Param Type 2', 'int', FALSE);;

SELECT setval('project.param_type_id_seq', max(id)) FROM project.param_type;

INSERT INTO project.project_param (project_id, param_id, is_deleted) VALUES
('11111111-1111-1111-1111-111111111111', 1, FALSE);

INSERT INTO project.section (id, project_id, name, is_deleted) VALUES
(1, '11111111-1111-1111-1111-111111111111', 'Section 1', FALSE);

SELECT setval('project.section_id_seq', max(id)) FROM project.section;

INSERT INTO project.section_param (section_id, param_id, is_deleted) VALUES
(1, 2, FALSE);
