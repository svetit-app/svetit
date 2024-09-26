FROM web_deps AS web
FROM nginx
WORKDIR /app
COPY nginx.conf /etc/nginx/nginx.conf
COPY headers.js /etc/nginx/headers.js
COPY server.nginx.conf /etc/nginx/conf.d/default.conf
COPY --from=web --chown=www-data:www-data /dist /app/
