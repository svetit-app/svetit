FROM python:3.10

WORKDIR /swagger2jsonschema
COPY doc/api/api.yaml .

RUN python3.10 -m pip install 'pip-tools >= 7, < 8'
RUN pip install swagger2jsonschema
RUN swagger2jsonschema --include-bodies --include-parameters api.yaml