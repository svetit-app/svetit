#!/bin/bash

set -m

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

make -C "$SCRIPT_PATH/../src/back/$1" service-start-debug &

sleep 3

psql -h localhost -p 15434 -U testsuite -d app_app < "$SCRIPT_PATH/../src/back/$1/tests/static/test_data.sql" || true

jobs

fg %1