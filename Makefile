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

run-%:
	make -C pipeline stop-$*
	make -C src/back/$* build-debug
	make -C src/back/$* service-start-debug

run-bin-%:
	make -C pipeline stop-$*
	make -C src/back/$* build-debug
	./src/back/$*/build_debug/svetit_$* \
		--config "./src/back/$*/configs/static_config.yaml" \
		--config_vars "./src/back/$*/configs/config_vars.yaml"

test-%:
	make -C src/back/$* test-debug

test-specific-%:
	make -C src/back/$* test-debug-specific $(specific)