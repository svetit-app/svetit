FROM archlinux/archlinux:base-devel AS builder

RUN mkdir -p /build/app/pkgs && pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Syu

COPY third_party/userver/scripts/docs/en/deps/arch.md ./

ARG depsfile=./arch.md
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
echo "nobody ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/nobody ;
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		DIR=$(mktemp -d) ;\
		git clone https://aur.archlinux.org/$REPLY.git $DIR ;\
		chmod -R 777 $DIR
		pushd $DIR ;\
		yes|sudo -u nobody makepkg -si --needed --noconfirm
		mv *.pkg.tar.zst /build/app/pkgs/
		popd ;\
		rm -rf $DIR ;\
	done ;
rm /etc/sudoers.d/nobody
EOF

COPY . /build/

RUN cd /build/third_party/userver && \
	git clean -dfx && \
	git reset --hard

ENV CMAKE_OPTIONS="-DCMAKE_INSTALL_PREFIX=/build/app -DCMAKE_BUILD_TYPE=Release"
RUN \
	cd /build/auth && \
	git init && \
	make build-release && \
	make install

# stage 2
FROM archlinux/archlinux:base

WORKDIR /app
COPY --from=builder /build/app .

RUN pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Syu

ARG depsfile=./arch.md
COPY deps.txt $depsfile
RUN echo "depsfile: '$depsfile'"

RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		pacman -U --noconfirm /app/pkgs/$REPLY-*.pkg.tar.zst || exit 1 ;\
	done ;
rm -fr /app/pkgs
EOF

ENTRYPOINT ["/app/bin/svetit_auth.sh"]

