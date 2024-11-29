#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

DIR_PATH="$SCRIPT_PATH/../../doc/api"

docker run --rm -v "$DIR_PATH":/spec redocly/cli lint --skip-rule=security-defined --skip-rule=info-license-url --skip-rule=operation-4xx-response --skip-rule=operation-2xx-response ./openapi.yaml
[ $? -ne 0 ] && exit 1

OUT_PATH="$DIR_PATH/bundled"
[ -d "$OUT_PATH" ] || mkdir -p "$OUT_PATH"

docker run --rm --user "$(id -u):$(id -g)" -v "$DIR_PATH":/spec redocly/cli bundle -o "./bundled/openapi.yaml" ./openapi.yaml
