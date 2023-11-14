#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

[ -z "$SPACE_LOG_LEVEL" ] && SPACE_LOG_LEVEL=debug
[ -z "$SPACE_PORT" ] && SPACE_PORT=8083

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
logger-level: $SPACE_LOG_LEVEL

is_testing: $TESTING

server-port: $SPACE_PORT
db-url: '$SPACE_DB_URL'

can-create: $SPACE_CAN_CREATE
default-space: $SPACE_DEFAULT
spaces-limit-for-user: $SPACE_LIMIT_FOR_USER
items-limit-for-list: $SPACE_ITEMS_LIMIT_FOR_LIST

EOF
