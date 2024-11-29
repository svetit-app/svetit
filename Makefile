
all: help

init init-env create-user up down stop migrate:
	@make -C pipeline $@

migrate-%:
	@make -C pipeline migrate-$*

docker-build:
	@make -C pipeline build

docker-build-%:
	@make -C pipeline build-$*

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

generate-angular-http:
	pipeline/generate-angular-http.sh

back-%:
	make -C src/back $*

help:
	@echo "# Описание команд:"
	@echo
	@echo "Сборка и установка зависимостей для разработки бэкенда"
	@echo " make back-deps-build"
	@echo " sudo make back-deps-install"
	@echo
	@echo "Работа с конкретным микросервисом"
	@echo " make back-(MS Name)-(MS Operation)"
	@echo " MS Names: auth, space, node, project"
	@echo " MS Operation: build-debug, install-debug, run-bin, test-specific, test"
	@echo " ### Например:"
	@echo "  make back-auth-build-debug"
	@echo "  make back-auth-test-specific-debug"

%: help
