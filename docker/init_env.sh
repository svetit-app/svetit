#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

[ -f "$SCRIPT_PATH/.env" ] && exit 0

function genPassword {
	dd if=/dev/urandom bs=128 count=1 2>/dev/null | basenc --base64url | head -c 48
}

cp "$SCRIPT_PATH"/.env{.example,}

PASS_KEYS="DB_PASS SSO_DB_PASS SSO_ADMIN_PASS AUTH_DB_PASS APP_DB_PASS OIDC_CLIENT_SECRET"
for i in $PASS_KEYS; do
	sed -i "s/{{$i}}/$(genPassword)/" "$SCRIPT_PATH/.env"
done

echo "New ENV file generated"
