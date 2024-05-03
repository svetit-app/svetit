#!/bin/bash

if [ -z "$1" ]; then
	>&2 echo "Usage: $0 {service_name}"
	exit 1
fi

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )
source "$SCRIPT_PATH/../.env"

"$SCRIPT_PATH/venv/bin/pgmigrate" -c "postgres://${APP_DB_USER}:${APP_DB_PASS}@localhost:15433/${APP_DB}" -d "${SCRIPT_PATH}/../../src/back/$1/db/app" -t latest migrate
[ $? -eq 0 ] || exit 1
