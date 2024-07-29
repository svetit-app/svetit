#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

[ -z "$NODES_LOG_LEVEL" ] && NODES_LOG_LEVEL=debug
[ -z "$NODES_PORT" ] && NODES_PORT=8085

[ -z "$NODES_DB_URL" ] && NODES_DB_URL="postgresql://${NODES_DB_USER}:${NODES_DB_PASS}@localhost:15433/${APP_DB}"

TESTING="false"
OUT_PATH="$SCRIPT_PATH/config_vars.yaml"

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"
shift # past argument

case $key in
--test)
	TESTING="true"
	;;
-o|--out)
	OUT_PATH="$1"
	shift ;;
-h|--help)
	echo ""
	echo "Help for call $0:"
	echo "  --test           : Enable testing. Default: false"
	echo "  -o, --out        : Output path. Default: $SCRIPT_PATH/config_vars.yaml"
	echo ""
	echo "Emample: sh $0 --test"
	exit 0
	;;

*)
	POSITIONAL+=("$key")
	;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters


cat<<EOF > "$OUT_PATH"
worker-threads: 4
worker-fs-threads: 2
logger-level: $NODES_LOG_LEVEL

is_testing: $TESTING

server-port: $NODES_PORT
db-url: '$NODES_DB_URL'

items-limit-for-list: $NODES_ITEMS_LIMIT_FOR_LIST
json-schemas-path: $JSON_SCHEMAS_PATH

EOF
