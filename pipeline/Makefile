.PHONY: *

default:
	make init migrate up

init-env:
	./init_env.sh
	../src/back/auth/configs/create_session_key.sh
	(set -a; . ./.env && ../src/back/auth/configs/config_vars_from_env.sh --test)
	(set -a; . ./.env && ../src/back/space/configs/config_vars_from_env.sh --test)
init:
	make init-env
	./keycloak/init.sh

create-user:
	./keycloak/create_user.sh

migrate:
	make migrate-auth
	make migrate-space

migrate-%:
	./migrate/migrate.sh $*

up:
	docker compose up -d
up-%:
	docker compose up -d $*

down:
	docker compose down

stop-%:
	docker compose stop $*

status:
	docker compose ps
build:
	docker buildx bake
build-%:
	docker buildx bake $*

logs-%:
	docker compose logs -f  $*
