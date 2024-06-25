FROM redocly/cli:latest

WORKDIR /spec
COPY . ./

RUN redocly lint --skip-rule=security-defined --skip-rule=info-license-url --skip-rule=operation-4xx-response --skip-rule=operation-2xx-response ./openapi.yaml

RUN redocly bundle -o api-bundled.yaml ./openapi.yaml