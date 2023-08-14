.PHONY: *

default down:
	make -C docker $@

run-%:
	make -C docker stop-$*
	make -C src/back/$* service-start-debug
