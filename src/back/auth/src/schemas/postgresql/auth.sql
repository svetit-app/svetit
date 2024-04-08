-- This file is not used now. Table created at start Repository component (in constructor)
CREATE TABLE IF NOT EXISTS states (
	id INT PRIMARY KEY NOT NULL,
	state TEXT NOT NULL,
	redirect_url TEXT NOT NULL
	created TIMESTAMPTZ NOT NULL DEFAULT NOW(),
);
