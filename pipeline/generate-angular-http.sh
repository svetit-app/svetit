#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

DOCKER_OPENAPI_PATH="/local/openapi.yaml"
DOCKER_OUTPUT_PATH="/local/src/web/src/app/api/"
PROJECT_GENERATED_PATH="$SCRIPT_PATH/../src/web/src/app"
PROJECT_GENERATED_PATH_FOR_CLEANING="$PROJECT_GENERATED_PATH/api"

SPLITTED_DIR_PATH="$SCRIPT_PATH/../doc/api"
TMP_PATH="/tmp/svetit/"
TMP_PATH_OPENAPI_PATHS="/tmp/svetit/paths"

[ -d "$TMP_PATH" ] || mkdir -p "$TMP_PATH"

cp -r ${SPLITTED_DIR_PATH}/* ${TMP_PATH}

for file in "$TMP_PATH_OPENAPI_PATHS"/*; do
    if [ -f "$file" ]; then
      expressions=(
        '/XUserHeader.yaml/d'
        ';/XSessionHeader.yaml/d'
        ';/XSpaceIdHeader.yaml/d'
        ';/XSpaceRoleHeader.yaml/d'
        ';s/handler-//g'
      )

      sed -i "$(echo ${expressions[@]} | tr -d ' ')" "$file"
      sed -i '/  parameters:/ { N; /  parameters:\n  responses:/ { s/  parameters:\n//; } }' "$file"
      sed -i '/  parameters:/ { N; /  parameters:\n  requestBody:/ { s/  parameters:\n//; } }' "$file"
    fi
done

docker run --rm --user "$(id -u):$(id -g)" \
  -v ${TMP_PATH}:/local openapitools/openapi-generator-cli generate \
  -i ${DOCKER_OPENAPI_PATH} \
  -g typescript-angular \
  -o ${DOCKER_OUTPUT_PATH} \
  --additional-properties ngVersion=16.2.14,fileNaming=kebab-case,withInterfaces=true --generate-alias-as-model

cp -r "${TMP_PATH}src/web/src/app/api" ${PROJECT_GENERATED_PATH}

rm -rf ${TMP_PATH}

rm -rf "$PROJECT_GENERATED_PATH_FOR_CLEANING/.openapi-generator"
rm -rf "$PROJECT_GENERATED_PATH_FOR_CLEANING/.gitignore"
rm -rf "$PROJECT_GENERATED_PATH_FOR_CLEANING/.openapi-generator-ignore"
rm -rf "$PROJECT_GENERATED_PATH_FOR_CLEANING/git_push.sh"
rm -rf "$PROJECT_GENERATED_PATH_FOR_CLEANING/README.md"