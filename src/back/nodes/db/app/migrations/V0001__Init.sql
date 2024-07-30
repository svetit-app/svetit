CREATE SCHEMA IF NOT EXISTS nodes;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE nodes.nodes (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL DEFAULT '',
	latitude FLOAT NOT NULL DEFAULT 0.0::double precision,
	longitude FLOAT NOT NULL DEFAULT 0.0::double precision,
	created_at TIMESTAMP NOT NULL DEFAULT NOW()
);

COMMENT ON TABLE nodes.nodes IS 'Nodes.';
COMMENT ON COLUMN nodes.nodes.id IS 'Primary key of Node.';
COMMENT ON COLUMN nodes.nodes.name IS 'Name of Node.';
COMMENT ON COLUMN nodes.nodes.description IS 'Text description of Node.';
COMMENT ON COLUMN nodes.nodes.latitude IS 'Geographical latitude of Node.';
COMMENT ON COLUMN nodes.nodes.longitude IS 'Geographical longitude of Node.';
COMMENT ON COLUMN nodes.nodes.created_at IS 'Timestamp of Node creation.';

CREATE TABLE nodes.node_project (
	node_id UUID NOT NULL REFERENCES nodes.nodes (id) ON DELETE CASCADE,
	project_id UUID NOT NULL,
	PRIMARY KEY(node_id, project_id)
);

COMMENT ON TABLE nodes.node_project IS 'Projects of Node.';
COMMENT ON COLUMN nodes.node_project.node_id IS 'Node which project belongs to.';
COMMENT ON COLUMN nodes.node_project.project_id IS 'Id of Project.';