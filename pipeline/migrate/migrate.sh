#!/bin/bash

if [ -z "$1" ]; then
	>&2 echo "Usage: $0 {service_name}"
	exit 1
fi

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )
source "$SCRIPT_PATH/../.env"

if [ ! -d "$SCRIPT_PATH/venv" ]; then
	mkdir "$SCRIPT_PATH/venv"
	python3 -m venv "$SCRIPT_PATH/venv/"
	"$SCRIPT_PATH/venv/bin/pip" install --disable-pip-version-check -U -r "$SCRIPT_PATH/requirements.txt"
	if [ $? -ne 0 ]; then
		>&2 echo "Failed virtual-env initialization"
		rm -fr "$SCRIPT_PATH/venv"
		exit 1
	fi
fi

"$SCRIPT_PATH/venv/bin/pgmigrate" -c "postgres://${APP_DB_USER}:${APP_DB_PASS}@localhost:15433/${APP_DB}" -d "${SCRIPT_PATH}/../../src/back/$1/db" -t latest migrate
[ $? -eq 0 ] || exit 1

