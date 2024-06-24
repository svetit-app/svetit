.PHONY: *

default init-env init create-user up down stop migrate:
	make -C pipeline $@

migrate-%:
	make -C pipeline migrate-$*

docker-build:
	make -C pipeline build

docker-build-%:
	make -C pipeline build-$*

docker-build-no-cache:
	make -C pipeline build-no-cache

docker-build-no-cache-%:
	make -C pipeline build-no-cache-$*

# docker-status docker-logs-% docker-bash-%
docker-%:
	make -C pipeline $*

up-%:
	make -C pipeline $@

down-%:
	make -C pipeline $@

build-%: generate-schemas
	make -C src/back/$* build-debug

run-%: generate-schemas
	make -C src/back/$* build-debug
	set -m; make -C src/back/$* service-start-debug & sleep 3 && ( psql -h localhost -p 15434 -U testsuite -d app_app < src/back/$*/tests/static/test_data.sql || true ) && jobs && fg %1

run-bin-%: generate-schemas
	make -C src/back/$* build-debug
	./pipeline/run_bin.sh $*

test-%: generate-schemas
	make -C src/back/$* test-debug

test-specific-%: generate-schemas
	make -C src/back/$* test-specific-debug $(specific)

generate-schemas:
	pipeline/schemas/generate.sh
