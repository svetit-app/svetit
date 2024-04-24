.PHONY: *

default init-env init create-user up down stop migrate:
	make -C pipeline $@

migrate-%:
	make -C pipeline migrate-$*

# docker-build-% docker-status
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
	make -C src/back/deps/swagger2jsonschema venv
	make -C src/back/deps/swagger2jsonschema build
	. src/back/deps/swagger2jsonschema/.venv/bin/activate && pip install src/back/deps/swagger2jsonschema/dist/openapi2jsonschema-0.11.0-py3-none-any.whl && openapi2jsonschema --include-bodies --include-parameters doc/api/api.yaml
