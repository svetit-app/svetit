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
	make -C src/back/$* service-start-debug

run-mocked-%: generate-schemas
	make -C src/back/$* build-debug
	./pipeline/run_with_mock.sh $*

run-bin-%: generate-schemas
	make -C src/back/$* build-debug
	./pipeline/run_bin.sh $*

run-proxy-for-gdb-%: generate-schemas
	./pipeline/run_proxy_for_gdb.sh $*

test-%: generate-schemas
	make -C src/back/$* test-debug

test-specific-%: generate-schemas
	make -C src/back/$* test-specific-debug $(specific)

generate-schemas:
	pipeline/schemas/generate.sh

generate-angular-http:
	pipeline/generate-angular-http.sh

build-deps:
	pushd src/back/deps/userver
	cmake -S./ -B./build_release -DCMAKE_BUILD_TYPE=Release -DUSERVER_INSTALL=ON -DUSERVER_IS_THE_ROOT_PROJECT=0 -DUSERVER_FEATURE_POSTGRESQL=1 -DUSERVER_FEATURE_PATCH_LIBPQ=0 -GNinja
	cmake -S./ -B./build_debug -DCMAKE_BUILD_TYPE=Debug -DUSERVER_INSTALL=ON -DUSERVER_SANITIZE="ub addr" -DUSERVER_IS_THE_ROOT_PROJECT=0 -DUSERVER_FEATURE_POSTGRESQL=1 -DUSERVER_FEATURE_PATCH_LIBPQ=0 -GNinja
	cmake --build build_debug/
	popd
	pushd src/back/deps/jwt-cpp 
	// TODO read docs/install.md
	popd
	pushd src/back/deps/svetit
	make build-debug
	popd

install-deps:
	cmake --install build_debug/
