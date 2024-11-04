FROM ghcr.io/userver-framework/ubuntu-22.04-userver-base@sha256:2b09c5b5592b726eefd5b33d6411eb0ed68eb0b4d39b6b2acbc53ecbdb41cec9
WORKDIR /deps

COPY deps.txt ./
RUN rm /etc/apt/sources.list.d/rabbitmq.list \
	&& apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends $(awk '{print $1}' deps.txt)

COPY . ./

RUN cd ./userver \
	&& rm -fr .git \
	&& git init \
	&& cmake -S./ -B./build_release -DCMAKE_BUILD_TYPE=Release -DUSERVER_INSTALL=ON -DUSERVER_IS_THE_ROOT_PROJECT=0 -DUSERVER_FEATURE_POSTGRESQL=1 -DUSERVER_FEATURE_PATCH_LIBPQ=0 -GNinja \
	&& cmake --build build_release/ \
	&& cmake --install build_release/ \
	&& cd .. \
	&& rm -fr userver

RUN cd ./jwt-cpp \
	&& cmake -S./ -B./build_release -DCMAKE_BUILD_TYPE=Release -DJWT_BUILD_EXAMPLES=0 -GNinja \
	&& cmake --build build_release/ \
	&& cmake --install build_release/ \
	&& cd .. \
	&& rm -fr jwt-cpp

RUN rm -fr /app/api-common-protos
