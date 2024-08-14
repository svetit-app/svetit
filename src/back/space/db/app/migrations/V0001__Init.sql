/* pgmigrate-encoding: utf-8 */
CREATE SCHEMA IF NOT EXISTS space;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE space.space (
	id UUID PRIMARY KEY DEFAULT UUID_GENERATE_V4(),
	name TEXT NOT NULL,
	key VARCHAR(36) NOT NULL,
	requests_allowed BOOLEAN NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT NOW()
);

CREATE UNIQUE INDEX idx_space_key ON space.space (key);

CREATE TABLE space.invitation (
	id SERIAL PRIMARY KEY,
	space_id UUID NOT NULL REFERENCES space.space (id),
	creator_id VARCHAR(40) NOT NULL,
	user_id VARCHAR(40) NOT NULL,
	role SMALLINT,
	created_at TIMESTAMP NOT NULL DEFAULT NOW()
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
	created_at TIMESTAMP NOT NULL DEFAULT NOW(),
	expired_at TIMESTAMP NOT NULL
);

CREATE TABLE space.user (
	space_id UUID NOT NULL REFERENCES space.space (id),
	user_id VARCHAR(40) NOT NULL,
	is_owner BOOLEAN NOT NULL,
	joined_at TIMESTAMP NOT NULL DEFAULT NOW(),
	role SMALLINT NOT NULL,

	PRIMARY KEY (space_id, user_id)
);

CREATE TABLE space.group (
	id SERIAL PRIMARY KEY,
	name VARCHAR(64) NOT NULL,
	description TEXT NOT NULL,
	space_id UUID NOT NULL REFERENCES space.space (id)
);

CREATE TABLE space.role (
	id SERIAL PRIMARY KEY,
	space_id UUID NOT NULL REFERENCES space.space (id),
	name VARCHAR(64) NOT NULL,
	is_system BOOLEAN DEFAULT FALSE
);

CREATE UNIQUE INDEX idx_system_role ON space.role (name) WHERE is_system = TRUE;

CREATE UNIQUE INDEX idx_not_system_role ON space.role (name, space_id) WHERE is_system = FALSE;