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

function keycloak_init() {
	docker run --rm -v "$SCRIPT_PATH/scripts":/scripts --net $NET -it --entrypoint /scripts/create_basics.sh $IMAGE \
		-u "$SSO_ADMIN" "$SSO_ADMIN_PASS" \
		-r "$OIDC_REALM" \
		-c "$OIDC_CLIENT_ID" \
		-s "$OIDC_CLIENT_SECRET" \
		$REDIRECTS
	RES=$?
	[ $RES -eq 0 ] && return 0

	if [ -z "$FORCE_SKIP_INIT_ERROR" ] && [ -z "$FORCE_RE_INIT_AT_ERROR" ] && [ -z "$FORCE_EXIT_AT_ERROR" ]; then
		echo ''
		echo '[NOTE]'
		echo ' -- You can force silent behavior by set environment variables:'
		echo " -- to skip: env FORCE_SKIP_INIT_ERROR=1 $0 ..."
		echo " -- to recreate volume: env FORCE_RE_INIT_AT_ERROR=1 $0 ..."
		echo " -- to prevent ask for actions: env FORCE_EXIT_AT_ERROR=1 $0 ..."
		echo ''

		echo 'Please select action:'
		echo '[0] Skip error. Continue initialization'
		echo '[1] Drop pipeline_pg_data docker volume and try again'
		echo '[2] Exit with error'
		echo 'Please, enter action number [2]: '
		read act_res
		echo ''

		if [ "$act_res" = "0" ]; then export FORCE_SKIP_INIT_ERROR=1
		elif [ "$act_res" = "1" ]; then export FORCE_RE_INIT_AT_ERROR=1
		else export FORCE_EXIT_AT_ERROR=1
		fi
	fi

	[ ! -z "$FORCE_EXIT_AT_ERROR" ] && [ "$FORCE_EXIT_AT_ERROR" != "0" ] && return $RES

	if [ ! -z "$FORCE_SKIP_INIT_ERROR" ] && [ "$FORCE_SKIP_INIT_ERROR" != "0" ]; then
		return 0
	fi

	if [ ! -z "$FORCE_RE_INIT_AT_ERROR" ] && [ "$FORCE_RE_INIT_AT_ERROR" != "0" ]; then
		export FORCE_RE_INIT_AT_ERROR=0
		export FORCE_EXIT_AT_ERROR=1
		docker volume rm pipeline_pg_data
		keycloak_init
		return $?
	fi

	return $RES
}

keycloak_init
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

