## Доступные службы
Веб-клиент: http://localhost:8080/
Кейклоак: http://localhost:8081/
pgAdmin: http://localhost:5050/

## Описание работы docker контейнеров

#### Запуск
```shell
docker compose up
```

#### Посмотреть что сейчас запущенно
```shell
docker compose ps
```

#### Взяв из колонки SERVICE вывода предыдущей команды имя сервиса, можно выполнить команду на конкретном запущенном контейнере
```shell
docker compose exec <service_name> bash
```

#### Чтобы узнать адрес какого то контейнера из контейнера можно воспользоваться curl
```shell
curl -vvv <service_name>
```

#### Volumes
Usually in /var/lib/docker/volumes/
```shell
# list
docker volume ls

# inspect container volumes
docker inspect -f "{{ .Mounts }}" <container_id>

# remove
docker volume rm <volume_name>
```

#### Очистка
```shell
docker system prune

# or
docker volume prune
docker image prune
docker container prune

# Удалить все остановленные контейнеры
docker rm -f $(docker ps -a -q)
```

#### Экспорт из контейнера кейклоак
```shell
/opt/keycloak/bin/kc.sh export --file /opt/jboss/keycloak/imports/realm-export.json
```
