FROM cpp_deps AS builder
WORKDIR /build

COPY . ./

RUN find /deps -maxdepth 1 -type d -not -path /deps -exec ln -sf {} third_party/ \;

RUN mkdir -p /app && \
	mv /deps/pkgs /app/ && \
	mv /deps/deps.txt /app/

ENV CMAKE_RELEASE_FLAGS="-DCMAKE_INSTALL_PREFIX=/app"
RUN \
	cp /deps/Makefile.local.archlinux Makefile.local && \
	git init && \
	make install


# stage 2
FROM archlinux/archlinux:base

WORKDIR /app
COPY --from=builder /app .

RUN echo 'Server = https://mirror.yandex.ru/archlinux/$repo/os/$arch' > /etc/pacman.d/mirrorlist

RUN pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Syy && \
	pacman --noconfirm -S archlinux-keyring && \
	pacman --noconfirm -S reflector

RUN reflector --latest 5 --country RU --protocol https --save "/etc/pacman.d/mirrorlist" --sort rate

ENV depsfile=/app/deps.txt

RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		pacman -U --noconfirm /app/pkgs/$REPLY-*.pkg.tar.zst || exit 1 ;\
	done ;
rm -fr /app/pkgs
EOF

RUN rm -fr /app/pkgs && rm $depsfile

ARG APP_NAME
ENV APP_NAME=${APP_NAME}
ENTRYPOINT /app/bin/svetit_${APP_NAME}.sh
