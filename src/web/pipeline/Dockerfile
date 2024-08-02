#STAGE 1
FROM node:20.5 AS build
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install --force && mkdir /dist
COPY . .

RUN \
	node_modules/.bin/ng build --configuration ru && mv dist/* /dist/ && \
	node_modules/.bin/ng build --configuration en && mv dist/* /dist/

RUN sh -c 'for i in `ls /dist/`; do \
		sed -ri "s/(src|href)=\"([^\/][^\"]+)\"/\1=\"\/$i\/\2\"/g" /dist/$i/index.html; \
	done'


#STAGE 2
FROM nginx:latest
WORKDIR /app
COPY ./pipeline/nginx.conf /etc/nginx/nginx.conf
COPY ./pipeline/headers.js /etc/nginx/headers.js
COPY ./pipeline/server.nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build --chown=www-data:www-data /dist /app/
