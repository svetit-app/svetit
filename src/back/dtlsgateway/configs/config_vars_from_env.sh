#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

[ -z "$DTLSGATEWAY_LOG_LEVEL" ] && DTLSGATEWAY_LOG_LEVEL=debug
[ -z "$DTLSGATEWAY_PORT" ] && DTLSGATEWAY_PORT=8083

[ -z "$DTLSGATEWAY_DB_URL" ] && DTLSGATEWAY_DB_URL="postgresql://${DTLSGATEWAY_DB_USER}:${DTLSGATEWAY_DB_PASS}@localhost:15433/${APP_DB}"

[ -z "$DTLSGATEWAY_DEFAULT" ] && DTLSGATEWAY_DEFAULT="|-"

TESTING="false"
OUT_PATH="$SCRIPT_PATH/config_vars.yaml"
TOKEN_KEY_PATH="$SCRIPT_PATH/token.key"

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
--token-key)
	TOKEN_KEY_PATH="$1"
	shift ;;
-h|--help)
	echo ""
	echo "Help for call $0:"
	echo "  --test           : Enable testing. Default: false"
	echo "  -o, --out        : Output path. Default: $SCRIPT_PATH/config_vars.yaml"
	echo "  --token-key    : Token key path. Default: $SCRIPT_PATH/token.key"
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
logger-level: $DTLSGATEWAY_LOG_LEVEL

is_testing: $TESTING

server-port: $DTLSGATEWAY_PORT
db-url: '$DTLSGATEWAY_DB_URL'

items-limit-for-list: $DTLSGATEWAY_ITEMS_LIMIT_FOR_LIST
internal-tls-key-path: $TOKEN_KEY_PATH
json-schemas-path: $JSON_SCHEMAS_PATH

EOF
