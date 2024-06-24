.PHONY: *

default init-env init create-user up down stop migrate:
	make -C pipeline $@

migrate-%:
	make -C pipeline migrate-$*

docker-build:
	rm -rf src/back/auth/build_release
	rm -rf src/back/space/build_release
	rm -rf src/back/project/build_release
	make -C pipeline build

docker-build-%:
	rm -rf src/back/$*/build_release
	make -C pipeline build-$*

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
	make -C src/back/$* service-start-debug

run-bin-%: generate-schemas
	make -C src/back/$* build-debug
	./pipeline/run_bin.sh $*

test-%: generate-schemas
	make -C src/back/$* test-debug

test-specific-%: generate-schemas
	make -C src/back/$* test-specific-debug $(specific)

generate-schemas:
	pipeline/schemas/generate.sh
