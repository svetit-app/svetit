-- This file is not used now. Table created at start Repository component (in constructor)
CREATE TABLE IF NOT EXISTS session (
	id SERIAL,
	token TEXT NOT NULL,
	created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
	expired TIMESTAMPTZ NOT NULL,
	userId VARCHAR(36) NOT NULL,
	device TEXT NOT NULL,
	accessToken TEXT NOT NULL,
	refreshToken TEXT NOT NULL,
	idToken TEXT NOT NULL,
	active BOOLEAN NOT NULL
);
