.PHONY: *

default:
	make init up

init:
	./init_env.sh
	./auth/init.sh
	./keycloak/init.sh

create-user:
	./keycloak/create_user.sh

up:
	docker compose up -d

down:
	docker compose down

build-%:
	docker compose build $*

stop-%:
	docker compose stop $*
