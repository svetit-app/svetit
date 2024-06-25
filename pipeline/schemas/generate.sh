#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

SPLITTED_DIR_PATH="$SCRIPT_PATH/../../doc/api"
SPLITTED_SAVED_HASH_PATH="$SPLITTED_DIR_PATH/.hash"
SPLITTED_SAVED_HASH=$(cat "$SPLITTED_SAVED_HASH_PATH" 2>/dev/null)

GENERATED_HASH=$(find "$SPLITTED_DIR_PATH" -type f ! -name '.hash' | xargs -I "{}" sha1sum "{}" | sha1sum | head -c 40)

[ "$GENERATED_HASH" = "$SPLITTED_SAVED_HASH" ] && exit 0

docker run --rm -v "$SPLITTED_DIR_PATH":/spec redocly/cli lint --skip-rule=security-defined --skip-rule=info-license-url --skip-rule=operation-4xx-response --skip-rule=operation-2xx-response ./openapi.yaml

[ $? -ne 0 ] && {
	exit 1;
}

API_YAML_PATH_HOST="/tmp/svetit/api.yaml"
API_YAML_PATH_GUEST="/tmp/api.yaml"
TMP_PATH="/tmp/svetit/"
[ -d "$TMP_PATH" ] || mkdir -p "$TMP_PATH"

docker run --rm -v "$SPLITTED_DIR_PATH":/spec -v "$TMP_PATH":/tmp redocly/cli bundle -o "$API_YAML_PATH_GUEST" ./openapi.yaml

SCHEMAS_PATH="$SCRIPT_PATH/../../schemas"
[ -d "$SCHEMAS_PATH" ] || mkdir -p "$SCHEMAS_PATH"

rm -fr "$SCHEMAS_PATH/*"

source "$SCRIPT_PATH/../venv/bin/activate"
swagger2jsonschema --include-bodies --include-parameters "$API_YAML_PATH_HOST"
[ $? -ne 0 ] && {
	rm -f "$API_YAML_PATH_HOST";
	exit 1;
}

rm -f "$API_YAML_PATH_HOST"

echo -n "$GENERATED_HASH" > "$SPLITTED_SAVED_HASH_PATH"
