.PHONY: *

default:
	make init up

init:
	./init_env.sh

up:
	docker-compose up

stop-%:
	docker-compose stop $*
