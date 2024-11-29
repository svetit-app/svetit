.PHONY: *

build:
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

install:
	cmake --install build_debug/

