.PHONY: *

default down:
	make -C pipeline $@

build-docker-%:
	make -C pipeline build-$*

run-%:
	make -C pipeline stop-$*
	make -C src/back/$* service-start-debug
