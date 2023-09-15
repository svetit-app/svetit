.PHONY: *

default down:
	make -C pipeline $@

run-%:
	make -C pipeline stop-$*
	make -C src/back/$* service-start-debug

run-space:
	make -C src/back/space service-start-debug
