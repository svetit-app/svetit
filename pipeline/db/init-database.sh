#!/bin/bash

function createItem() {
	if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ]; then
		>&2 echo "Credentials is empty u:'$1' p:'$2' db:'$3'"
		return
	else
		>&2 echo "Prepare db initialization for user '$1'"
	fi
	local username="$1" password="$2" dbname="$3"

	cat<<EOL
CREATE USER $username WITH PASSWORD '$password';
CREATE DATABASE $dbname;
\connect $dbname;
CREATE SCHEMA AUTHORIZATION $username;
GRANT ALL PRIVILEGES ON DATABASE $dbname TO $username;
EOL
}

set -e

psql -v ON_ERROR_STOP=1 --username "$POSTGRES_USER" --dbname "$POSTGRES_DB" <<-EOSQL
$(createItem ${SSO_USER} ${SSO_PASS} ${SSO_DB})
$(createItem ${AUTH_USER} ${AUTH_PASS} ${AUTH_DB})
$(createItem ${APP_USER} ${APP_PASS} ${APP_DB})
$(createItem ${SPACE_USER} ${SPACE_PASS} ${SPACE_DB})
EOSQL
