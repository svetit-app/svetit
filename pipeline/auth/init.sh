#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )
OUT_PATH="$SCRIPT_PATH/session.key"

[ -f "$OUT_PATH" ] && exit 0

openssl genrsa -out "$OUT_PATH" 2048
