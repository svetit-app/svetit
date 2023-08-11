#STAGE 1
FROM node:latest AS build
WORKDIR /usr/src/app
COPY package.json package-lock.json ./
RUN npm install --force
COPY . .
RUN npm run build

#STAGE 2
FROM nginx:latest
COPY ./docker/nginx.conf /etc/nginx/nginx.conf
COPY ./docker/headers.js /etc/nginx/headers.js
COPY ./docker/server.nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=build /usr/src/app/dist/* /usr/share/nginx/html/

