.PHONY: *

default:
	make init up

init-env:
	./init_env.sh
	./auth/init.sh
init:
	make init-env
	./keycloak/init.sh

create-user:
	./keycloak/create_user.sh

up:
	docker compose up -d

down:
	docker compose down

stop-%:
	docker compose stop $*

status:
	docker compose ps
build:
	docker compose build
build-%:
	docker compose build $*
