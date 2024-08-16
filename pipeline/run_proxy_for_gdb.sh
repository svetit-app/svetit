#!/bin/bash

SCRIPT_PATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

getAppPath() {
	local res="$SCRIPT_PATH/../src/back/$1/build_debug"
	[ -d "$res" ] && echo "$res"
}

APP=$1
APP_PATH=$(getAppPath $APP)
if [ -z "$APP" ] || [ -z "$APP_PATH" ]; then
	>&2 echo "Usage: $0 {app_name}"
	>&2 echo "App '$APP' must be built"
	exit 1
fi

isApiGwHostsReplaced=0
apiGwName=svetit_web

checkIsPortOpen() {
	ss -ltn | grep $1 >/dev/null
	return $?
}

genExtraHostsArgs() {
	local res=
	local data=$(grep '_PORT=' "$SCRIPT_PATH/.env")
	for i in $data; do
		[[ "$i" =~ ([A-Za-z_]+)_PORT[^0-9]*([0-9]+) ]] || continue
		name="${BASH_REMATCH[1],,}"
		port="${BASH_REMATCH[2]}"

		# check is app folder exists
		[ -z "$(getAppPath $name)" ] && continue

		checkIsPortOpen $port
		[ $? -eq 0 ] || continue

		res+=" --add-host $name:host-gateway"
	done
	echo "$res"
}

genExtraHostsArgsExceptApp() {
	local res=
	local data=$(grep '_PORT=' "$SCRIPT_PATH/.env")
	for i in $data; do
		[[ "$i" =~ ([A-Za-z_]+)_PORT[^0-9]*([0-9]+) ]] || continue
		name="${BASH_REMATCH[1],,}"
		port="${BASH_REMATCH[2]}"

		if [ "$APP" == "$name" ]; then
			continue;
		fi

		# check is app folder exists
		[ -z "$(getAppPath $name)" ] && continue

		checkIsPortOpen $port
		[ $? -eq 0 ] || continue

		res+=" --add-host $name:host-gateway"
	done
	echo "$res"
}

replaceApiGatewayHostsAfterAppStarted() {
	# break if Api Gateway container is not running
	if [ "$( docker container inspect -f '{{.State.Running}}' $apiGwName )" != "true" ]; then
		echo "API Gateway ports can't be replaced because container stoped"
		return 0
	fi
	echo -e "\033[0;36mBegin replace API Gateway hosts\033[0m"

	docker stop $apiGwName svetit_$APP
	docker rm $apiGwName 2>&1 >/dev/null

	extraHosts=$(genExtraHostsArgs)
	docker run --rm --net=svetit_app -p 8080:80 $extraHosts -d --name $apiGwName $apiGwName

	echo -e "\033[1;32mApi gateway hosts replaced\033[0m"
	isApiGwHostsReplaced=1
}

restoreApiGatewayHosts() {
	[ $isApiGwHostsReplaced -eq 1 ] || return 0
	isApiGwHostsReplaced=0
	extraHosts=$(genExtraHostsArgsExceptApp)
	if [ -n "$extraHosts" ]; then
		docker stop $apiGwName
		docker compose -f "$SCRIPT_PATH/docker-compose.yml" up -d $APP
		sleep 3
		docker run --rm --net=svetit_app -p 8080:80 $extraHosts -d --name $apiGwName $apiGwName
		echo "Api gateway host for $APP returned"
	else
		docker stop $apiGwName
		docker rm $apiGwName 2>&1 >/dev/null
		docker compose -f "$SCRIPT_PATH/docker-compose.yml" up -d web $APP
		echo "Api gateway hosts returned"
	fi
}

# enable jobs manage
set -m

# catch Ctrl+C
trap 'restoreApiGatewayHosts' SIGINT

echo "Starting proxy for GDB, app should be running already"
echo "Also please run it with other make run-bin-% runned services carefully."

replaceApiGatewayHostsAfterAppStarted

while true
do
	sleep 1
done

restoreApiGatewayHosts
