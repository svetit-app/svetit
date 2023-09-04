#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

# check is submodules empty
if [ "$(ls "$SCRIPT_PATH/../src/back/third_party/userver")" = "" ]; then
	git submodule update --init --recursive
fi

[ -f "$SCRIPT_PATH/.env" ] && exit 0

function genPassword {
	dd if=/dev/urandom bs=128 count=1 2>/dev/null | basenc --base64url | head -c 48
}

cp "$SCRIPT_PATH"/.env{.example,}

PASS_KEYS="DB_PASS SSO_DB_PASS SSO_ADMIN_PASS AUTH_DB_PASS APP_DB_PASS OIDC_CLIENT_SECRET"
for i in $PASS_KEYS; do
	password=$(genPassword)
	sed -i "s/{{$i}}/$password/" "$SCRIPT_PATH/.env"
	if [ $i = "OIDC_CLIENT_SECRET" ]; then
		sed -i "s/\"secret\":.*/\"secret\": \"$password\",/g" "$SCRIPT_PATH/keycloak/import/realm-export.json"
	fi
done


echo "New ENV file generated"