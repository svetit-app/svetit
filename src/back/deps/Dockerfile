FROM ghcr.io/userver-framework/ubuntu-22.04-userver-base@sha256:8716600bfdae9b5ad47c3340f3cc50566b1a3458cfd9c1c318f2dc1252b9d29f
WORKDIR /deps

COPY . ./

RUN rm -rf /app/api-common-protos

RUN cd ./userver && \
	rm -fr .git && \
	git init