FROM redocly/cli:latest

WORKDIR /spec
COPY ./splitted/ ./splitted/

RUN redocly bundle -o api-bundled.yaml ./splitted/openapi.yaml