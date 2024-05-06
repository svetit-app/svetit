FROM archlinux/archlinux:base-devel
WORKDIR /deps

RUN echo 'Server = https://mirror.yandex.ru/archlinux/$repo/os/$arch' > /etc/pacman.d/mirrorlist

RUN mkdir -p /deps/pkgs && pacman-key --init
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -Sy archlinux-keyring && \
	pacman --noconfirm -S reflector

RUN reflector --country RU --latest 5 --protocol https --save "/etc/pacman.d/mirrorlist" --sort rate

COPY userver/scripts/docs/en/deps/arch.md ./

ENV depsfile=/deps/arch.md
RUN --mount=type=cache,target=/var/cache/pacman \
	pacman --noconfirm -S --needed $(cat $depsfile | grep -v -- 'makepkg|' | tr '\n' ' ')

RUN <<EOF
echo "nobody ALL=(ALL:ALL) NOPASSWD: ALL" > /etc/sudoers.d/nobody ;
cat $depsfile | grep -oP 'makepkg\|\K.*' | while read ;\
	do \
		DIR=$(mktemp -d) ;\
		git clone https://aur.archlinux.org/$REPLY.git $DIR ;\
		chmod -R 777 $DIR ;\
		pushd $DIR ;\
		yes|sudo -u nobody makepkg -si --needed --noconfirm ;\
		mv *.pkg.tar.zst /deps/pkgs/ ;\
		popd ;\
		rm -rf $DIR ;\
	done ;
rm /etc/sudoers.d/nobody
EOF

COPY . ./

RUN cd ./userver && \
	rm -fr .git && \
	git init

RUN rm $depsfile
