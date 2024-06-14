FROM ubuntu:22.04
WORKDIR /deps

ARG APP_NAME
ARG APP_FOLDER_PATH
ENV APP_NAME=${APP_NAME}
ENV APP_FOLDER_PATH=./${APP_NAME}/

COPY deps/userver/scripts/docs/en/deps/ubuntu-22.04.md /userver_tmp/
COPY deps/userver/scripts/docker/setup-base-ubuntu-22.04-env.sh /userver_tmp/

COPY deps/userver/scripts/external_deps/requirements.txt  /userver_tmp/requirements/external-deps.txt
COPY deps/userver/testsuite/requirements-postgres.txt     /userver_tmp/requirements/postgres.txt
COPY deps/userver/testsuite/requirements-testsuite.txt    /userver_tmp/requirements/testsuite.txt
COPY deps/userver/testsuite/requirements.txt              /userver_tmp/requirements/testsuite-base.txt

COPY deps/userver/scripts/docker/pip-install.sh           /userver_tmp/

RUN ( \
  cd /userver_tmp \
  && ./setup-base-ubuntu-22.04-env.sh \
  && ./pip-install.sh \
  && mkdir /app && cd /app \
  && rm -rf /userver_tmp \
)

COPY ./deps/ ./

RUN cd ./userver && \
	rm -fr .git && \
	git init

WORKDIR /app
COPY ${APP_FOLDER_PATH} ./

RUN find /deps -maxdepth 1 -type d -not -path /deps -exec ln -sf {} third_party/ \;

ENV CMAKE_RELEASE_FLAGS="-DCMAKE_INSTALL_PREFIX=/app"
RUN \
	git init && \
	make install

COPY --from=schemas_generator /swagger2jsonschema/schemas/* /app/schemas/

ENTRYPOINT /app/bin/svetit_${APP_NAME}.sh