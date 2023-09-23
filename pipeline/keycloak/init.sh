#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )
DIR_NAME=$(basename "$(dirname "$SCRIPT_PATH")")

[ -f "$SCRIPT_PATH/.initialized" ] && exit 0

NET=${DIR_NAME}_app
IMAGE=${DIR_NAME}-sso
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

echo "Begin initialize keycloak"
REDIRECTS=''
for i in $OIDC_REDIRECT_URIS; do
	REDIRECTS+=" --ru $i"
done

docker run --rm -v "$SCRIPT_PATH/scripts":/scripts --net $NET -it --entrypoint /scripts/create_basics.sh $IMAGE \
	-u "$SSO_ADMIN" "$SSO_ADMIN_PASS" \
	-r "$OIDC_REALM" \
	-c "$OIDC_CLIENT_ID" \
	-s "$OIDC_CLIENT_SECRET" \
	$REDIRECTS
RES=$?
if [ $RES -ne 0 ]; then
	>&2 echo "Failed keycloak initialization"
	exit $RES
fi

touch "$SCRIPT_PATH/.initialized"

[ -z "$OIDC_TEST_USER" ] && exit 0

while [ -z "$OIDC_TEST_USER_PASS" ]; do
	echo -n "Please enter user '$OIDC_TEST_USER' password: "
	read -s OIDC_TEST_USER_PASS
	echo ''
done

docker run --rm -v "$SCRIPT_PATH/scripts":/scripts --net $NET -it --entrypoint /scripts/create_user.sh $IMAGE \
	-u "$SSO_ADMIN" "$SSO_ADMIN_PASS" \
	-r "$OIDC_REALM" \
	-c "$OIDC_CLIENT_ID" \
	--nu "$OIDC_TEST_USER" "$OIDC_TEST_USER_PASS"
RES=$?
[ $RES -ne 0 ] && >&2 echo "Failed to create test user"
exit $RES

