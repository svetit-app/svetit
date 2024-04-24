FROM python:3.10

COPY src/back/deps/swagger2jsonschema ./swagger2jsonschema
COPY doc/api/api.yaml ./swagger2jsonschema

WORKDIR /swagger2jsonschema

RUN python3.10 -m pip install 'pip-tools >= 7, < 8'
RUN pip install -r requirements.txt
RUN pip install -r requirements-dev.txt
RUN pip install .
RUN openapi2jsonschema --include-bodies --include-parameters api.yaml