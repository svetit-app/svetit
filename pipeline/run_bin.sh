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

isRunning() {
	kill -0 $pid &>/dev/null
	return $?
}

isApiGwHostsReplaced=0
apiGwName=svetit_web

getPortByAppName() {
	grep '_PORT=' "$SCRIPT_PATH/.env" | grep -i $APP | grep -oP "[0-9]+"
}

checkIsPortOpen() {
	ss -ltn | grep $1 >/dev/null
	return $?
}

waitForAppStarted() {
	local port=$(getPortByAppName)
	if [ -z "$port" ]; then
		>&2 echo "Port for app '$APP' doesn't exist in config"
		return 1
	fi
	while isRunning; do
		checkIsPortOpen $port
		[ $? -eq 0 ] && break
		sleep 0.5
	done
	return 0
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

replaceApiGatewayHostsAfterAppStarted() {
	# break if Api Gateway container is not running
	if [ "$( docker container inspect -f '{{.State.Running}}' $apiGwName )" != "true" ]; then
		echo "API Gateway ports can't be replaced becose container stoped"
		return 0
	fi
	echo -e "\033[0;36mBegin replace API Gateway hosts\033[0m"

	waitForAppStarted
	if [ $? -ne 0 ]; then
		>&2 echo "Wait for app started failed"
		return 1
	fi

	if ! isRunning; then
		echo "App stoped. Break"
		return 0
	fi

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
	docker stop $apiGwName
	docker rm $apiGwName 2>&1 >/dev/null
	docker compose -f "$SCRIPT_PATH/docker-compose.yml" up -d web $APP
	echo "Api gateway hosts returned"

	# return to app
	fg %1 &>/dev/null
}

# enable jobs manage
set -m

# catch Ctrl+C
trap 'restoreApiGatewayHosts' SIGINT

echo "Start $APP"
cd "$APP_PATH"
exec ./svetit_$APP \
	--config "../configs/static_config.yaml" \
	--config_vars "../configs/config_vars.yaml" &
echo "Started"

pid=$!
sleep 1.5

if ! isRunning; then
	>&2 echo "Start app failed. PID: $pid"
	fg %1
	exit 1
fi

replaceApiGatewayHostsAfterAppStarted

jobs
echo -e "\033[0;36mReturn to app\033[0m"
fg %1

restoreApiGatewayHosts
