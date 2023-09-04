.PHONY: *

default:
	make init up

init:
	./init_env.sh

up:
	docker compose up -d

down:
	docker compose down

stop-%:
	docker compose stop $*
