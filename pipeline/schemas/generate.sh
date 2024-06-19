#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

SPLITTED_DIR_PATH="$SCRIPT_PATH/../../doc/api/splitted"
SPLITTED_SAVED_HASH_PATH="$SPLITTED_DIR_PATH/../.hash"
SPLITTED_SAVED_HASH=$(cat "$SPLITTED_SAVED_HASH_PATH" 2>/dev/null)

GENERATED_HASH=$(find "$SPLITTED_DIR_PATH" -type f | xargs -I "{}" sha1sum "{}" | sha1sum | head -c 40)

[ "$GENERATED_HASH" = "$SPLITTED_SAVED_HASH" ] && exit 0

docker run --rm -v "$SPLITTED_DIR_PATH":/spec redocly/cli bundle -o api-bundled.yaml ./openapi.yaml

API_YAML_PATH="$SCRIPT_PATH/../../doc/api/api.yaml"

mv -f "$SPLITTED_DIR_PATH"/api-bundled.yaml "$API_YAML_PATH"

SCHEMAS_PATH="$SCRIPT_PATH/../../schemas"
[ -d "$SCHEMAS_PATH" ] || mkdir -p "$SCHEMAS_PATH"

rm -fr "$SCHEMAS_PATH/*"

source "$SCRIPT_PATH/../venv/bin/activate"
swagger2jsonschema --include-bodies --include-parameters "$API_YAML_PATH"
[ $? -ne 0 ] && exit $?

echo -n "$GENERATED_HASH" > "$SPLITTED_SAVED_HASH_PATH"
