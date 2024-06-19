.PHONY: *

default:
	make init migrate up

init-env:
	./init_env.sh
	../src/back/auth/configs/create_session_key.sh
	(set -a; . ./.env && ../src/back/auth/configs/config_vars_from_env.sh --test)
	(set -a; . ./.env && ../src/back/space/configs/config_vars_from_env.sh --test)
	(set -a; . ./.env && ../src/back/project/configs/config_vars_from_env.sh --test)
init:
	make init-env
	./keycloak/init.sh

create-user:
	./keycloak/create_user.sh

migrate:
	make migrate-auth
	make migrate-space
	make migrate-project

migrate-%:
	./migrate/migrate.sh $*

up:
	IS_DEPS_READY=$(shell docker images -q cpp_deps 2> /dev/null); \
		[ -z "$$IS_DEPS_READY" ] && make build || true
	docker compose up -d
up-%:
	IS_DEPS_READY=$(shell docker images -q cpp_deps 2> /dev/null); \
		[ -z "$$IS_DEPS_READY" ] && make build-$* || true
	docker compose up -d $*

down:
	docker compose down

down-%:
	docker compose down $*

stop-%:
	docker compose stop $*

status:
	docker compose ps
build:
	docker buildx bake
build-%:
	docker buildx bake $*

logs-%:
	docker compose logs -f $*

bash-%:
	docker compose exec -ti $* bash
