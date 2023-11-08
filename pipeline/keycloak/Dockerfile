# syntax=docker/dockerfile:1
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

WORKDIR /opt/keycloak
# for demonstration purposes only, please make sure to use proper certificates in production instead
RUN keytool -genkeypair -storepass password -storetype PKCS12 -keyalg RSA -keysize 2048 -dname "CN=server" -alias server -ext "SAN:c=DNS:localhost,IP:127.0.0.1" -keystore conf/server.keystore
RUN /opt/keycloak/bin/kc.sh build

FROM quay.io/keycloak/keycloak:latest
COPY --from=builder /opt/keycloak/ /opt/keycloak/

COPY scripts/healthcheck.sh /opt/
# TODO: REPLACE interval=5s TO interval=5m AFTER docker v25 released
HEALTHCHECK --interval=5s --timeout=3s --start-period=30s --start-interval=5s \
	CMD /opt/healthcheck.sh

ENTRYPOINT ["/opt/keycloak/bin/kc.sh"]

