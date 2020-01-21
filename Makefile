#
# C-Dogs for the RetroFW
#
# by pingflood; 2019
#

CHAINPREFIX 	:= /opt/mipsel-RetroFW-linux-uclibc

# To create a debug build, run `cmake -D CMAKE_BUILD_TYPE=Debug .` instead

# Update repo properly (should be handled by most git GUI clients)
# git submodule init
# git submodule update --init --recursive
# git submodule update --recursive

all:
	mkdir -p /tmp/cdogs
	cmake "GCW0=1" -DCMAKE_TOOLCHAIN_FILE="$(CHAINPREFIX)/usr/share/buildroot/toolchainfile.cmake" -B/tmp/cdogs -H.
	cd /tmp/cdogs && make
	cp /tmp/cdogs/src/cdogs-sdl cdogs/cdogs.dge

clean:
	rm -rf /tmp/cdogs

ipk: all
	@rm -rf /tmp/.cdogs-ipk/ && mkdir -p /tmp/.cdogs-ipk/root/home/retrofw/games/cdogs /tmp/.cdogs-ipk/root/home/retrofw/apps/gmenu2x/sections/games
	@cp -r cdogs/cdogs.elf cdogs/cdogs.png cdogs/data cdogs/dogfights cdogs/graphics cdogs/missions cdogs/music cdogs/sounds /tmp/.cdogs-ipk/root/home/retrofw/games/cdogs
	@cp cdogs/cdogs.lnk /tmp/.cdogs-ipk/root/home/retrofw/apps/gmenu2x/sections/games
	@sed "s/^Version:.*/Version: $$(date +%Y%m%d)/" cdogs/control > /tmp/.cdogs-ipk/control
	@cp cdogs/conffiles /tmp/.cdogs-ipk/
	@tar --owner=0 --group=0 -czvf /tmp/.cdogs-ipk/control.tar.gz -C /tmp/.cdogs-ipk/ control conffiles
	@tar --owner=0 --group=0 -czvf /tmp/.cdogs-ipk/data.tar.gz -C /tmp/.cdogs-ipk/root/ .
	@echo 2.0 > /tmp/.cdogs-ipk/debian-binary
	@ar r cdogs/cdogs.ipk /tmp/.cdogs-ipk/control.tar.gz /tmp/.cdogs-ipk/data.tar.gz /tmp/.cdogs-ipk/debian-binary

opk: all
	@mksquashfs \
	cdogs/default.retrofw.desktop \
	cdogs/cdogs.dge \
	cdogs/cdogs.png \
	cdogs/data \
	cdogs/dogfights \
	cdogs/graphics \
	cdogs/missions \
	cdogs/music \
	cdogs/sounds \
	cdogs/cdogs.opk \
	-all-root -noappend -no-exports -no-xattrs
