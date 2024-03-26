#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

NET=svetit_app
IMAGE=svetit_sso
CONTAINER=svetit_sso

WAIT_SSO_TIMEOUT=90

STATUS=$(docker inspect -f {{.State.Health.Status}} $CONTAINER)
if [ "$STATUS" != "healthy" ]; then
	if [ "$STATUS" != "starting" ]; then
		docker compose -f "$SCRIPT_PATH/../docker-compose.yml" up -d sso
	fi

	D1=$(date +%s)
	echo -n 'Wait for sso started'
	while [ "$STATUS" != "healthy" ]; do
		D2=$(date +%s)
		if [ $((D2-D1)) -gt $WAIT_SSO_TIMEOUT ]; then
			>&2 echo -e "\nWait sso timeout"
			exit 1
		fi

		echo -n '.'
		sleep 1
		STATUS=$(docker inspect -f {{.State.Health.Status}} $CONTAINER)
	done
	echo ''
fi

source "$SCRIPT_PATH/../.env"

echo "Create new user"

while [ -z "$USER_NAME" ]; do
	echo -n "Please enter username: "
	read USER_NAME
	echo ''
done

while [ -z "$USER_PASS" ]; do
	echo -n "Please enter user '$USER_NAME' password: "
	read -s USER_PASS
	echo ''
done

echo -n "First name [default: $USER_NAME]: "
read -s FIRST_NAME
echo ''

echo -n "Last name [default: $FIRST_NAME]: "
read -s LAST_NAME
echo ''

echo -n "Email [default: $USER_NAME@example.com]: "
read -s EMAIL
echo ''

docker run --rm -v "$SCRIPT_PATH/scripts":/scripts --net $NET -it --entrypoint /scripts/create_user.sh $IMAGE \
	-u "$SSO_ADMIN" "$SSO_ADMIN_PASS" \
	-r "$OIDC_REALM" \
	-c "$OIDC_CLIENT_ID" \
	--nu "$USER_NAME" "$USER_PASS" \
	--firstName "$FIRST_NAME" \
	--lastName "$LAST_NAME" \
	--email "$EMAIL"
RES=$?
[ $RES -ne 0 ] && >&2 echo "Failed to create test user"
exit $RES

