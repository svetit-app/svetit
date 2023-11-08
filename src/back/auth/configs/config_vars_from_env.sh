#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

[ -z "$AUTH_LOG_LEVEL" ] && AUTH_LOG_LEVEL=debug
[ -z "$AUTH_PORT" ] && AUTH_PORT=8082

[ -z "$AUTH_DB_URL" ] && AUTH_DB_URL="postgresql://${AUTH_DB_USER}:${AUTH_DB_PASS}@localhost:15433/${AUTH_DB}"

[ -z "$OIDC_CLIENT_ID" ] && OIDC_CLIENT_ID=web
[ -z "$OIDC_CLIENT_SECRET" ] && OIDC_CLIENT_SECRET=
[ -z "$OIDC_PROVIDER_URL" ] && OIDC_PROVIDER_URL=http://localhost:8081/realms/svetit

TESTING="false"
OUT_PATH="$SCRIPT_PATH/config_vars.yaml"
CONFIG_FALLBACK_PATH="$SCRIPT_PATH/dynamic_config_fallback.json"
SESSION_KEY_PATH="$SCRIPT_PATH/session.key"

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
--dynamic-config)
	CONFIG_FALLBACK_PATH="$1"
	shift ;;
--session-key)
	SESSION_KEY_PATH="$1"
	shift ;;
-h|--help)
	echo ""
	echo "Help for call $0:"
	echo "  --test           : Enable testing. Default: false"
	echo "  -o, --out        : Output path. Default: $SCRIPT_PATH/config_vars.yaml"
	echo "  --dynamic-config : Dynamic config path. Default: $SCRIPT_PATH/dynamic_config_fallback.json"
	echo "  --session-key    : Session key path. Default: $SCRIPT_PATH/session.key"
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
logger-level: $AUTH_LOG_LEVEL

is_testing: $TESTING

config-fallback-path: $CONFIG_FALLBACK_PATH

server-port: $AUTH_PORT
db-url: '$AUTH_DB_URL'

oidc-client-id: $OIDC_CLIENT_ID
oidc-client-secret: $OIDC_CLIENT_SECRET
oidc-provider-url: $OIDC_PROVIDER_URL

internal-tls-key-path: $SESSION_KEY_PATH

token-cookie-max-age: 15
EOF
