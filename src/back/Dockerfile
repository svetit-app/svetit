FROM archlinux/archlinux:base-devel AS builder

RUN pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Syu

COPY third_party/userver/scripts/docs/en/deps/arch.md ./

ARG depsfile=./arch.md

RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
echo "nobody ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/nobody ;
mkdir -p /opt/svetit/pkgs
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		DIR=$(mktemp -d) ;\
		git clone https://aur.archlinux.org/$REPLY.git $DIR ;\
		chmod -R 777 $DIR
		pushd $DIR ;\
		yes|sudo -u nobody makepkg -si --needed --noconfirm
		mv *.pkg.tar.zst /opt/svetit/pkgs/
		popd ;\
		rm -rf $DIR ;\
	done ;
rm /etc/sudoers.d/nobody
EOF

WORKDIR /svetit
COPY . .

RUN <<EOF
pushd third_party/userver ;\
git apply ../../userver.patch ;\
popd
EOF

ENV CMAKE_OPTIONS="-DCMAKE_INSTALL_PREFIX=/opt/svetit -DCMAKE_BUILD_TYPE=Release"
RUN \
	mkdir -p /opt/svetit && \
	cd ./auth && \
	git init && \
	make build-release && \
	make install

# stage 2
FROM archlinux/archlinux:base
COPY --from=builder /opt/svetit /opt/svetit

RUN pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Syu

COPY ./deps.txt /opt/svetit/pkgs/deps.txt

ARG depsfile=/opt/svetit/pkgs/deps.txt
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		pacman -U --noconfirm /opt/svetit/pkgs/$REPLY-*.pkg.tar.zst ;\
	done ;
rm -fr /opt/svetit/pkgs
EOF

ENTRYPOINT ["/opt/svetit/bin/svetit_auth.sh"]

