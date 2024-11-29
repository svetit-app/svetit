.PHONY: *

generate-schemas:
	../../pipeline/schemas/generate.sh

%: generate-schemas
	@[[ "$@" =~ ^([^-]+)-(.*) ]] && make -C $${BASH_REMATCH[1]} $${BASH_REMATCH[2]}

run-mocked-%: generate-schemas
	make -C $* build-debug
	../../pipeline/run_with_mock.sh $*

run-bin-%: generate-schemas
	make -C $* build-debug
	../../pipeline/run_bin.sh $*

run-proxy-for-gdb-%: generate-schemas
	../../pipeline/run_proxy_for_gdb.sh $*
