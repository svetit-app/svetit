#!/bin/bash

export PATH="/opt/keycloak/bin:$PATH"

KC_HOST=http://sso:8080
USERNAME=admin
PASSWORD=
REALM=svetit
CLIENT_ID=web
CLIENT_SECRET=
ROLE=svetit-role
REDIRECTS=()
DEFAULT_RURI=('https://svetit.io/api/*' 'http://localhost:8080/api/*' 'http://localhost:8082/*')

if [ "$#" -le 0 ]; then
	echo -e "\nSomething is missing... Type \"sh $0 -h\" without the quotes to find out more...\n"
	exit 0
fi

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
	-r|--realm)
	REALM="$2"
	shift # past argument
	shift # past value
	;;
	-c|--client_id)
	CLIENT_ID="$2"
	shift # past argument
	shift # past value
	;;
	-s|--secret)
	CLIENT_SECRET="$2"
	shift # past argument
	shift # past value
	;;
	--ru|--redirectUri)
	REDIRECTS+=("$2")
	shift # past argument
	shift # past value
	;;
	--role)
	NAME="$2"
	shift # past argument
	shift # past value
	;;
	-u|--user)
	USERNAME="$2"
	PASSWORD="$3"
	if [ PASSWORD == "" ]; then
		echo "ERROR: $1 need 2 parameters"
		exit
	fi
	shift # past argument
	shift # past value 1
	shift # past value 2
	;;
	-h|--help)
	echo ""
	echo "Help for call $0:"
	echo "  -h, --host          : Keycloak URL. Default: http://sso:8080"
	echo "  -u, --user          : 2 params: login password of master admin user"
	echo "  -r, --realm         : name of realm. Default: svetit"
	echo "  -c, --client_id     : name of client. Default: web"
	echo "  -s, --secret        : Client secret."
	echo "  --ru, --redirectUri : Keycloak URL. Default: $DEFAULT_RURI"
	echo "  --role              : name of default role. Default: svetit-role"
	echo ""
	echo "Emample: sh $0 -u Administrator password --secret qwerty"
	exit 0
	;;

	*)
	POSITIONAL+=("$1")
	shift
	;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [ -z "$USERNAME" ] || [ -z "$PASSWORD" ] || [ -z "$CLIENT_SECRET" ]; then
	>&2 echo "Parameters missing. Use -h for more info"
	exit 1
fi

echo "[login] Server: $KC_HOST User: $USERNAME"
kcadm.sh config credentials --server "$KC_HOST" --realm master --user "$USERNAME" --password "$PASSWORD"
if [ $? -ne 0 ]; then
	>&2 echo "Failed to login"
	exit 1
fi

echo ""
echo "[create] Realm: $REALM"
kcadm.sh create realms -s realm="$REALM" -s enabled=true -o

echo ""
echo "[create] Client: $CLIENT_ID"

RURI=''
[ ${#REDIRECTS[@]} -eq 0 ] && REDIRECTS=(${DEFAULT_RURI[@]})
for i in "${!REDIRECTS[@]}"; do
	[ $i -eq 0 ] || RURI+=','
	RURI+="\"${REDIRECTS[i]}\""
	echo "[create] - redirectUri: ${REDIRECTS[i]}"
done
kcadm.sh create clients -r "$REALM" \
	-s clientId="$CLIENT_ID" \
	-s secret="$CLIENT_SECRET" \
	-s publicClient="false" \
	-s "redirectUris=[$RURI]" \
	-s enabled=true

echo ""
echo "[create] Role: $ROLE"
kcadm.sh create roles -r "$REALM" -s name="$ROLE"

