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

build-%:
	make -C src/back/$* build-debug

run-%:
	make -C src/back/$* build-debug
	make -C src/back/$* service-start-debug

run-bin-%:
	make -C src/back/$* build-debug
	./pipeline/run_bin.sh $*

test-%:
	make -C src/back/$* test-debug

test-specific-%:
	make -C src/back/$* test-specific-debug $(specific)
