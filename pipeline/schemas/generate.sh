#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

DST_PATH="$SCRIPT_PATH/../../schemas"
DST_HASH_PATH="$DST_PATH/.hash"
DST_HASH=$(cat "$DST_HASH_PATH" 2>/dev/null)
[ -d "$DST_PATH" ] || mkdir -p "$DST_PATH"

SRC_PATH="$SCRIPT_PATH/../../doc/api/api.yaml"
SRC_HASH=$(sha1sum "$SRC_PATH" | awk '{print $1}')

[ "$SRC_HASH" = "$DST_HASH" ] && exit 0

rm -fr "$DST_PATH/*"

source "$SCRIPT_PATH/../venv/bin/activate"
swagger2jsonschema --include-bodies --include-parameters "$SRC_PATH"
[ $? -ne 0 ] && exit $?

echo -n "$SRC_HASH" > "$DST_HASH_PATH"
