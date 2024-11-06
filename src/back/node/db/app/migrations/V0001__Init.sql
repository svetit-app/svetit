CREATE SCHEMA IF NOT EXISTS node;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE node.node (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	space_id UUID NOT NULL,
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL DEFAULT '',
	latitude FLOAT NOT NULL DEFAULT 0.0::double precision,
	longitude FLOAT NOT NULL DEFAULT 0.0::double precision,
	created_at BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT
);

COMMENT ON TABLE node.node IS 'Nodes.';
COMMENT ON COLUMN node.node.id IS 'Primary key of Node.';
COMMENT ON COLUMN node.node.space_id IS 'Space Id which node belongs to.';
COMMENT ON COLUMN node.node.name IS 'Name of Node.';
COMMENT ON COLUMN node.node.description IS 'Text description of Node.';
COMMENT ON COLUMN node.node.latitude IS 'Geographical latitude of Node.';
COMMENT ON COLUMN node.node.longitude IS 'Geographical longitude of Node.';
COMMENT ON COLUMN node.node.created_at IS 'Timestamp of Node creation.';

CREATE TABLE node.node_project (
	node_id UUID NOT NULL REFERENCES node.node (id) ON DELETE CASCADE,
	project_id UUID NOT NULL,
	PRIMARY KEY(node_id, project_id)
);

COMMENT ON TABLE node.node_project IS 'Projects of Node.';
COMMENT ON COLUMN node.node_project.node_id IS 'Node which project belongs to.';
COMMENT ON COLUMN node.node_project.project_id IS 'Id of Project.';

CREATE TABLE node.node_group (
	node_id UUID NOT NULL REFERENCES node.node (id) ON DELETE CASCADE,
	group_id INT NOT NULL,
	PRIMARY KEY(node_id, group_id)
);

COMMENT ON TABLE node.node_group IS 'Groups of Node.';
COMMENT ON COLUMN node.node_group.node_id IS 'Node which group associates with.';
COMMENT ON COLUMN node.node_group.group_id IS 'Id of group.';

CREATE TABLE node.group_user (
	group_id INT NOT NULL,
	user_id VARCHAR(40) NOT NULL,
	PRIMARY KEY(group_id, user_id)
);

COMMENT ON TABLE node.group_user IS 'Users of groups.';
COMMENT ON COLUMN node.group_user.group_id IS 'Group which user belongs to.';
COMMENT ON COLUMN node.group_user.user_id IS 'Id of user.';
