/* pgmigrate-encoding: utf-8 */
CREATE SCHEMA IF NOT EXISTS space;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE space.space (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	name TEXT NOT NULL,
	key VARCHAR(36) NOT NULL,
	requests_allowed BOOLEAN NOT NULL,
	created_at BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT
);

CREATE UNIQUE INDEX idx_space_key ON space.space (key);

CREATE TABLE space.role (
	id SERIAL PRIMARY KEY,
	space_id UUID REFERENCES space.space (id),
	name VARCHAR(64) NOT NULL
);

CREATE UNIQUE INDEX idx_system_role ON space.role (name) WHERE space_id = NULL;
CREATE UNIQUE INDEX idx_not_system_role ON space.role (name, space_id) WHERE space_id != NULL;

INSERT INTO space.role (id, space_id, name) VALUES
	(1, NULL, 'admin'),
	(2, NULL, 'user'),
	(3, NULL, 'operator');
SELECT setval('space.role_id_seq', max(id)) FROM space.role;

CREATE TABLE space.invitation (
	id SERIAL PRIMARY KEY,
	space_id UUID NOT NULL REFERENCES space.space (id),
	creator_id VARCHAR(40) NOT NULL,
	user_id VARCHAR(40) NOT NULL,
	role_id INT REFERENCES space.role (id),
	created_at BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT
);

CREATE UNIQUE INDEX idx_space_invitation ON space.invitation (space_id, user_id);

CREATE FUNCTION check_user() RETURNS trigger AS $$
BEGIN
	PERFORM * FROM space.user WHERE user_id = NEW.user_id AND space_id = NEW.space_id;
	IF FOUND THEN
		RETURN NULL;
	END IF;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER check_user_presents
	BEFORE INSERT ON space.invitation
	FOR EACH ROW EXECUTE PROCEDURE check_user();

CREATE TABLE space.link (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	space_id UUID NOT NULL REFERENCES space.space (id),
	creator_id VARCHAR(40) NOT NULL,
	name TEXT NOT NULL,
	created_at BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT,
	expired_at BIGINT NOT NULL
);

CREATE TABLE space.user (
	space_id UUID NOT NULL REFERENCES space.space (id),
	user_id VARCHAR(40) NOT NULL,
	is_owner BOOLEAN NOT NULL,
	joined_at BIGINT NOT NULL DEFAULT EXTRACT(EPOCH FROM CURRENT_TIMESTAMP)::BIGINT,
	role_id INT REFERENCES space.role (id),

	PRIMARY KEY (space_id, user_id)
);

CREATE TABLE space.group (
	id SERIAL PRIMARY KEY,
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL,
	space_id UUID NOT NULL REFERENCES space.space (id)
);
