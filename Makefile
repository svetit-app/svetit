.PHONY: *

default init-env init create-user up down stop migrate:
	make -C pipeline $@

migrate-%:
	make -C pipeline migrate-$*

# docker-build-% docker-status
docker-%:
	make -C pipeline $*

run-%:
	make -C pipeline stop-$*
	make -C pipeline stop-db
	make -C src/back/$* service-start-debug

run-bin-%:
	make -C pipeline stop-$*
	make -C src/back/$* build-debug
	(set -a; . ./pipeline/.env && ./src/back/$*/build_debug/svetit_$*.sh)

test-%:
	make -C pipeline stop-db
	make -C src/back/$* test-debug