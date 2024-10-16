FROM cpp_deps AS builder
WORKDIR /build

COPY . ./

ENV CMAKE_RELEASE_FLAGS="-DCMAKE_INSTALL_PREFIX=/app"
RUN \
	git init && \
	make install

# stage 2
FROM ubuntu:22.04

WORKDIR /app

COPY --from=builder /app .

COPY --from=builder /deps/deps.txt deps.txt
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends $(awk '{print $1}' deps.txt) && rm -f deps.txt

COPY --from=schemas_generator /swagger2jsonschema/schemas/* /app/schemas/

ARG APP_NAME
ENV APP_NAME=${APP_NAME}
ENTRYPOINT /app/bin/svetit_${APP_NAME}.sh
