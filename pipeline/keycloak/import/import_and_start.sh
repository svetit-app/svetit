#!/bin/sh

sed -i "s/\"secret\":\\s\?\"[^\"]*\"/\"secret\": \"${OIDC_CLIENT_SECRET}\"/g" "/opt/keycloak/data/import/realm-export.json"

sh /opt/keycloak/bin/kc.sh start-dev --import-realm --hostname-url=http://localhost:8081