FROM ghcr.io/userver-framework/ubuntu-22.04-userver-base@sha256:267b03a367ea4d0d2a2e4620d81f60431c238e056342ef532163235cc6610ff5
WORKDIR /deps

COPY deps.txt ./
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends $(awk '{print $1}' deps.txt)

COPY . ./

RUN cd ./userver \
	&& rm -fr .git \
	&& git init \
	&& mkdir /deps/dist \
	&& cmake -S./ -B./build_release -DCMAKE_BUILD_TYPE=Release -DUSERVER_INSTALL=ON -DUSERVER_IS_THE_ROOT_PROJECT=0 -DUSERVER_FEATURE_POSTGRESQL=1 -DUSERVER_FEATURE_PATCH_LIBPQ=0 -DCMAKE_INSTALL_PREFIX=/deps/dist -GNinja \
	&& cmake --build build_release/ \
	&& cmake --install build_release/ \
	&& cd .. \
	&& rm -fr userver

RUN cd ./jwt-cpp \
	&& cmake -S./ -B./build_release -DCMAKE_BUILD_TYPE=Release -DJWT_BUILD_EXAMPLES=0 -DCMAKE_INSTALL_PREFIX=/deps/dist -GNinja \
	&& cmake --build build_release/ \
	&& cmake --install build_release/ \
	&& cd .. \
	&& rm -fr jwt-cpp

RUN rm -fr /app/api-common-protos
