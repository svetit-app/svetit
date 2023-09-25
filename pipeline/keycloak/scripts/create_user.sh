#!/bin/bash

export PATH="/opt/keycloak/bin:$PATH"

KC_HOST=http://sso:8080
USERNAME=admin
PASSWORD=
REALM=svetit
CLIENT_ID=web
ROLE=svetit-role
USER_NAME=test
USER_PASS=qwerty

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
	--nu|--newUser)
	USER_NAME="$2"
	USER_PASS="$3"
	if [ PASSWORD == "" ]; then
		echo "ERROR: $1 need 2 parameters"
		exit
	fi
	shift # past argument
	shift # past value 1
	shift # past value 2
	;;
	--firstName)
	FIRST_NAME="$2"
	shift # past argument
	shift # past value
	;;
	--lastName)
	LAST_NAME="$2"
	shift # past argument
	shift # past value
	;;
	--email)
	EMAIL="$2"
	shift # past argument
	shift # past value
	;;
	-h|--help)
	echo ""
	echo "Help for call $0:"
	echo "  -h, --host          : Keycloak URL. Default: http://sso:8080"
	echo "  -u, --user          : 2 params: login password of master admin user"
	echo "  --nu, --newUser     : 2 params: login password of new user"
	echo "  -r, --realm         : name of realm. Default: svetit"
	echo "  -c, --client_id     : name of client. Default: web"
	echo "  --role              : name of default role. Default: svetit-role"
	echo "  --firstName         : first name of user. Default same as login"
	echo "  --lastName          : last name of user. Default same as firstName"
	echo "  --email             : email of user. Default: {login}@example.com"
	echo ""
	echo "Emample: sh $0 -u Administrator password --newUser test qwerty"
	exit 0
	;;

	*)
	POSITIONAL+=("$1")
	shift
	;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [ -z "$USERNAME" ] || [ -z "$PASSWORD" ]; then
	>&2 echo "Parameters missing. Use -h for more info"
	exit 1
fi

kcadm.sh config credentials --server "$KC_HOST" --realm master --user "$USERNAME" --password "$PASSWORD"
if [ $? -ne 0 ]; then
	>&2 echo "Failed to login"
	exit 1
fi

[ -z "$FIRST_NAME" ] && FIRST_NAME="$USER_NAME"
[ -z "$LAST_NAME" ] && LAST_NAME="$FIRST_NAME"
[ -z "$EMAIL" ] && EMAIL="$USER_NAME@example.com"

kcadm.sh create users -r "$REALM" \
	-s username="$USER_NAME" \
	-s firstName="$FIRST_NAME" \
	-s lastName="$LAST_NAME" \
	-s email="$EMAIL" \
	-s enabled=true
kcadm.sh set-password -r "$REALM" --username "$USER_NAME" --new-password "$USER_PASS"
kcadm.sh add-roles -r "$REALM" --uusername "$USER_NAME" --rolename "$ROLE"

