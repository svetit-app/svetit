CMAKE_COMMON_FLAGS ?= -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_DEBUG_FLAGS ?= -DUSERVER_SANITIZE='ub addr'
CMAKE_RELEASE_FLAGS ?=
NPROCS ?= $(shell nproc)
CLANG_FORMAT ?= clang-format
SERVICE_NAME = svetit

# NOTE: use Makefile.local to override the options defined above.
-include Makefile.local

CMAKE_DEBUG_FLAGS += -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS)
CMAKE_RELEASE_FLAGS += -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS)

.PHONY: all
all: test-debug test-release

# Run cmake
.PHONY: cmake-debug
cmake-debug:
	cmake -B build_debug $(CMAKE_DEBUG_FLAGS)

.PHONY: cmake-release
cmake-release:
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
		((test -t 1 && GTEST_COLOR=1 ctest -V) || ctest -V)

# Cleanup data
.PHONY: clean-debug clean-release
clean-debug clean-release: clean-%:
	cmake --build build_$* --target clean

.PHONY: dist-clean
dist-clean:
	rm -rf build_*

# Install
.PHONY: install-debug install-release
install-debug install-release: install-%: build-%
	cmake --install build_$* -v

.PHONY: uninstall-debug uninstall-release
uninstall-debug uninstall-release: uninstall-%:
	xargs rm < build_$*/install_manifest.txt

# chaotic generations
.PHONY: chgen
chgen:
	chaotic-gen \
		-n "/components/schemas/([^/]*)/=svetit::api::{0}" \
		-f "(.*)={0}" \
		--plain-object-path-map '.*/([^/]+)\.yaml$$=/components/schemas/{0}/' \
		--clang-format=clang-format \
		--generate-serializers \
		--output-dir src/api \
		--relative-to=$(PWD)/../../../../doc/api/components/schemas \
		$(PWD)/../../../../doc/api/components/schemas/*.yaml
	find $(PWD}/src/api -name "*.hpp" -exec mv {} $(PWD)/include/svetit/api/ \;

# Format the sources
.PHONY: format
format:
	find src -name '*pp' -type f | xargs $(CLANG_FORMAT) -i
