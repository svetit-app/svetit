CREATE SCHEMA IF NOT EXISTS nodes;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE nodes.nodes (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL DEFAULT '',
	geo POINT NOT NULL
);

COMMENT ON TABLE nodes.nodes IS 'Nodes.';
COMMENT ON COLUMN nodes.nodes.id IS 'Primary key of Node.';
COMMENT ON COLUMN nodes.nodes.name IS 'Name of Node.';
COMMENT ON COLUMN nodes.nodes.description IS 'Text description of Node.';
COMMENT ON COLUMN nodes.nodes.geo IS 'Geographical coordinates of Node.';

CREATE TABLE nodes.node_project (
	node_id UUID NOT NULL REFERENCES nodes.nodes (id) ON DELETE CASCADE,
	project_id UUID NOT NULL,
	PRIMARY KEY(node_id, project_id)
);

COMMENT ON TABLE nodes.node_project IS 'Projects of Node.';
COMMENT ON COLUMN nodes.node_project.node_id IS 'Node which project belongs to.';
COMMENT ON COLUMN nodes.node_project.project_id IS 'Id of Project.';