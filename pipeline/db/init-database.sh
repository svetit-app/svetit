#!/bin/bash

function createDB() {
	if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
		>&2 echo "Credentials is empty u:'$1' p:'$2' db:'$3'"
		return
	fi
	local username="$1" password="$2" dbname="$3"
	>&2 echo "Prepare initialization for user '$username' db: '$dbname'"

	cat<<EOL
CREATE DATABASE $dbname;
\connect $dbname;
CREATE ROLE $username WITH LOGIN PASSWORD '$password';
CREATE SCHEMA AUTHORIZATION $username;
GRANT ALL PRIVILEGES ON DATABASE $dbname TO $username;
EOL
}

function createUser() {
	if [ -z "$1" ] || [ -z "$2" ]; then
		>&2 echo "Credentials is empty u:'$1' p:'$2'"
		return
	fi
	local username="$1" password="$2"
	>&2 echo "Prepare initialization for user '$username'"

	cat<<EOL
CREATE ROLE $username WITH INHERIT LOGIN PASSWORD '$password';
GRANT $GRP TO $username;
EOL
}

set -e

GRP=svetit_group

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
$(createDB ${SSO_USER} ${SSO_PASS} ${SSO_DB})
$(createDB ${APP_USER} ${APP_PASS} ${APP_DB})

\connect ${APP_DB};
CREATE ROLE $GRP WITH INHERIT NOLOGIN;
GRANT ALL ON SCHEMA public TO $GRP;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON TABLES TO $GRP;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT ALL ON SEQUENCES TO $GRP;

CREATE TABLE IF NOT EXISTS u_clients (
	hostname TEXT PRIMARY KEY,
	updated TIMESTAMPTZ NOT NULL,
	max_connections INTEGER NOT NULL
);

$(createUser ${AUTH_USER} ${AUTH_PASS})
$(createUser ${SPACE_USER} ${SPACE_PASS})
EOSQL
