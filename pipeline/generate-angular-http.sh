#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

PROJECT_ROOT_PATH="$SCRIPT_PATH/../"
DOCKER_OPENAPI_PATH="/local/doc/api/openapi.yaml"
DOCKER_OUTPUT_PATH="/local/src/web/src/app/api/"
PROJECT_GENERATED_PATH="$SCRIPT_PATH/../src/web/src/app/api"

docker run --rm --user "$(id -u):$(id -g)" \
  -v ${PROJECT_ROOT_PATH}:/local openapitools/openapi-generator-cli generate \
  -i ${DOCKER_OPENAPI_PATH} \
  -g typescript-angular \
  -o ${DOCKER_OUTPUT_PATH} \
  --additional-properties ngVersion=16.2.14,fileNaming=kebab-case,withInterfaces=true --generate-alias-as-model

rm -rf "$PROJECT_GENERATED_PATH/.openapi-generator"
rm -rf "$PROJECT_GENERATED_PATH/.gitignore"
rm -rf "$PROJECT_GENERATED_PATH/.openapi-generator-ignore"
rm -rf "$PROJECT_GENERATED_PATH/git_push.sh"
rm -rf "$PROJECT_GENERATED_PATH/README.md"
rm -rf "$PROJECT_ROOT_PATH/openapitools.json"