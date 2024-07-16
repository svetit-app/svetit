#STAGE 1
FROM node:20.5 AS build
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install --force && mkdir /dist
COPY . .
RUN cp ./src/environments/environment.prod.ts ./src/environments/environment.prod.ts.backup
RUN sed -i 's/BUILD_TAG_VERSION/btv12345/g' ./src/environments/environment.prod.ts
RUN node_modules/.bin/ng build --configuration ru && mv dist/* /dist/
RUN mv ./src/environments/environment.prod.ts.backup ./src/environments/environment.prod.ts
RUN sed -i 's/BUILD_TAG_VERSION/btv98765/g' ./src/environments/environment.prod.ts
RUN node_modules/.bin/ng build --configuration en && mv dist/* /dist/

#STAGE 2
FROM nginx:latest
WORKDIR /app
COPY ./pipeline/nginx.conf /etc/nginx/nginx.conf
COPY ./pipeline/headers.js /etc/nginx/headers.js
COPY ./pipeline/server.nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build --chown=www-data:www-data /dist /app/
