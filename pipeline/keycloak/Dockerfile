FROM quay.io/keycloak/keycloak:latest as builder

ARG DB_HOST
ARG DB_USER
ARG DB_PASS
ARG DB_NAME

# Enable health and metrics support
ENV KC_HEALTH_ENABLED=true
ENV KC_METRICS_ENABLED=true

# Configure a database vendor
ENV KC_DB=postgres
ENV KC_DB_URL=jdbc:postgresql://$DB_HOST/$DB_NAME
ENV KC_DB_USERNAME=$DB_USER
ENV KC_DB_PASSWORD=$DB_PASS
ENV KC_DB_SCHEMA=$DB_SCHEMA

COPY import/realm-export.json /opt/keycloak/data/import/realm-export.json

COPY import/import_and_start.sh /opt/keycloak/data/import/import_and_start.sh
USER root
RUN chown keycloak:root /opt/keycloak/data/import/import_and_start.sh
RUN chmod +x /opt/keycloak/data/import/import_and_start.sh
RUN chown keycloak:keycloak /opt/keycloak/data/import/realm-export.json
RUN chown keycloak:keycloak /opt/keycloak/data/import
USER keycloak

WORKDIR /opt/keycloak
# for demonstration purposes only, please make sure to use proper certificates in production instead
RUN keytool -genkeypair -storepass password -storetype PKCS12 -keyalg RSA -keysize 2048 -dname "CN=server" -alias server -ext "SAN:c=DNS:localhost,IP:127.0.0.1" -keystore conf/server.keystore
RUN /opt/keycloak/bin/kc.sh build

FROM quay.io/keycloak/keycloak:latest
COPY --from=builder /opt/keycloak/ /opt/keycloak/

ENTRYPOINT ["/opt/keycloak/data/import/import_and_start.sh"]

