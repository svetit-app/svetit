CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= -DUSERVER_SANITIZE='ub'
CMAKE_RELEASE_FLAGS ?=
NPROCS ?= $(shell nproc)
CLANG_FORMAT ?= clang-format
DOCKER_COMPOSE ?= docker-compose
SERVICE_NAME = svetit_project

# NOTE: use Makefile.local to override the options defined above.
-include Makefile.local

CMAKE_DEBUG_FLAGS += -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS)
CMAKE_RELEASE_FLAGS += -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS)

.PHONY: all
all: test-debug test-release

# Run cmake
.PHONY: cmake-debug
cmake-debug:
	git submodule update --init
	cmake -B build_debug $(CMAKE_DEBUG_FLAGS)

.PHONY: cmake-release
cmake-release:
	git submodule update --init
	cmake -B build_release $(CMAKE_RELEASE_FLAGS)

build_debug/CMakeCache.txt: cmake-debug
build_release/CMakeCache.txt: cmake-release

# Build using cmake
.PHONY: build-debug build-release
build-debug build-release: build-%: build_%/CMakeCache.txt
	cmake --build build_$* -j $(NPROCS) --target $(SERVICE_NAME)

# Test
.PHONY: test-debug test-release
test-debug test-release: test-%: build-%
	cmake --build build_$* -j $(NPROCS) --target $(SERVICE_NAME)_unittest
	cmake --build build_$* -j $(NPROCS) --target $(SERVICE_NAME)_benchmark
	cd build_$* && \
		. venv-testsuite-$(SERVICE_NAME)/bin/activate && \
		export TESTSUITE_POSTGRESQL_PORT=15434 && \
		((test -t 1 && GTEST_COLOR=1 PYTEST_ADDOPTS="--color=yes" ctest -V) || ctest -V)
	pep8 --ignore=W191,W504 tests

# Test specific test
.PHONY: test-specific-debug test-specific-release
test-specific-debug test-specific-release: test-specific-%: build-%
	cd build_$* && \
		. venv-testsuite-$(SERVICE_NAME)/bin/activate && \
		export TESTSUITE_POSTGRESQL_PORT=15434 && \
		./runtests-testsuite-$(SERVICE_NAME) -vvx ../tests/ -k "$(specific)"

# Start the service (via testsuite service runner)
.PHONY: service-start-debug service-start-release
service-start-debug service-start-release: service-start-%:
	. build_$*/venv-testsuite-$(SERVICE_NAME)/bin/activate && \
		export TESTSUITE_POSTGRESQL_PORT=15434 && \
		cmake --build build_$* -v --target=start-$(SERVICE_NAME)

# Cleanup data
.PHONY: clean-debug clean-release
clean-debug clean-release: clean-%:
	cmake --build build_$* --target clean

.PHONY: dist-clean
dist-clean:
	rm -rf build_*
	rm -f ./configs/config_vars.yaml
	rm -rf tests/__pycache__/
	rm -rf tests/.pytest_cache/

# Install
.PHONY: install-debug install-release
install-debug install-release: install-%: build-%
	cmake --install build_$* -v --component $(SERVICE_NAME)

.PHONY: install
install: install-release

# Format the sources
.PHONY: format
format:
	find src -name '*pp' -type f | xargs $(CLANG_FORMAT) -i
	find tests -name '*.py' -type f | xargs autopep8 -i

# Internal hidden targets that are used only in docker environment
.PHONY: --in-docker-start-debug --in-docker-start-release
--in-docker-start-debug --in-docker-start-release: --in-docker-start-%: install-%
	/home/user/.local/bin/$(SERVICE_NAME) \
		--config /home/user/.local/etc/$(SERVICE_NAME)/static_config.yaml \
		--config_vars /home/user/.local/etc/$(SERVICE_NAME)/config_vars.yaml

# Build and run service in docker environment
.PHONY: docker-start-service-debug docker-start-service-release
docker-start-service-debug docker-start-service-release: docker-start-service-%:
	$(DOCKER_COMPOSE) run -p 8082:8082 --rm $(SERVICE_NAME)-container $(MAKE) -- --in-docker-start-$*

# Start specific target in docker environment
.PHONY: docker-cmake-debug docker-build-debug docker-test-debug docker-clean-debug docker-install-debug docker-cmake-release docker-build-release docker-test-release docker-clean-release docker-install-release
docker-cmake-debug docker-build-debug docker-test-debug docker-clean-debug docker-install-debug docker-cmake-release docker-build-release docker-test-release docker-clean-release docker-install-release: docker-%:
	$(DOCKER_COMPOSE) run --rm $(SERVICE_NAME)-container $(MAKE) $*

# Stop docker container and cleanup data
.PHONY: docker-clean-data
docker-clean-data:
	$(DOCKER_COMPOSE) down -v
	rm -rf ./.pgdata
