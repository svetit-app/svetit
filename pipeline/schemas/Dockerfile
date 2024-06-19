FROM python:3.10

RUN pip install swagger2jsonschema

WORKDIR /swagger2jsonschema
COPY --from=redocly_custom /spec/api-bundled.yaml ./api.yaml

RUN swagger2jsonschema --include-bodies --include-parameters api.yaml
