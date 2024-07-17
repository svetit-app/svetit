#STAGE 1
FROM node:20.5 AS build
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install --force && mkdir /dist
COPY . .
RUN cp ./src/environments/environment.prod.ts ./src/environments/environment.prod.ts.backup

RUN export RAND1=$(openssl rand -base64 12 | tr -dc A-Za-z0-9) && export RAND2=$(openssl rand -base64 12 | tr -dc A-Za-z0-9) \
&& sed -i "s/BUILD_TAG_VERSION/$RAND1/g" ./src/environments/environment.prod.ts \
&& node_modules/.bin/ng build --configuration ru && mv dist/* /dist/ \
&& mv ./src/environments/environment.prod.ts.backup ./src/environments/environment.prod.ts \
&& sed -i "s/BUILD_TAG_VERSION/$RAND2/g" ./src/environments/environment.prod.ts \
&& node_modules/.bin/ng build --configuration en && mv dist/* /dist/

#STAGE 2
FROM nginx:latest
WORKDIR /app
COPY ./pipeline/nginx.conf /etc/nginx/nginx.conf
COPY ./pipeline/headers.js /etc/nginx/headers.js
COPY ./pipeline/server.nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build --chown=www-data:www-data /dist /app/
