-- This file is not used now. Table created at start Repository component (in constructor)
CREATE TABLE IF NOT EXISTS session (
	id SERIAL,
	token TEXT NOT NULL,
	created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
	expired TIMESTAMPTZ NOT NULL,
	user_id VARCHAR(36) NOT NULL,
	device TEXT NOT NULL,
	access_token TEXT NOT NULL,
	refresh_token TEXT NOT NULL,
	id_token TEXT NOT NULL,
	active BOOLEAN NOT NULL
);
