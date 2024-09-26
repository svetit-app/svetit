FROM node:20.5 AS build
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install --force && mkdir /dist
COPY . .

RUN \
	node_modules/.bin/ng build --configuration ru && mv dist/browser/* /dist/ && \
	node_modules/.bin/ng build --configuration en && mv dist/browser/* /dist/

# В файлах index.html подставляем код языка в начало пути к каждому включённому файлу
RUN sh -c 'for i in `ls /dist/`; do \
		sed -ri "s/(src|href)=\"([^\/][^\"]+)\"/\1=\"\/$i\/\2\"/g" /dist/$i/index.html; \
	done'
