.PHONY: *

default down:
	make -C pipeline $@

run-%:
	make -C pipeline stop-$*
	make -C src/back/$* service-start-debug
