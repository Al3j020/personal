# Copyright 2021 Roku, Inc.  All rights reserved.
#
# This Makefile is different from the other samples to show one way to fetch and
# build a library like FFMPEG.
COMMON := ../common
include $(COMMON)/Roku4.mk

# Define target binaries we want to build.
FFMPEG_ARCHIVE := ffmpeg-snapshot.tar.bz2
PATH_FFMPEG := ffmpeg
PATH_STAGING := $(abspath staging)
PATH_PACKAGE := package
PACKAGE := ./idk_sample_video.squashfs.bin
OBJECTS := main.o buffer.o screen.o
CONFIGURE_OPTIONS := --prefix=$(PATH_STAGING) \
	--enable-gpl --enable-version3 --enable-nonfree \
	--enable-small --disable-programs --enable-static --disable-shared \
	--disable-doc --disable-devices --disable-sdl2 --disable-filters --disable-encoders \
	--enable-runtime-cpudetect --enable-pthreads --enable-openssl \
	--extra-cflags="-I$(PATH_USR)/include" --extra-ldflags="-L$(PATH_USR)/lib" \
	--cross-prefix=$(CROSS_COMPILE) --enable-cross-compile \
	--arch=armv7-a --cpu=cortex-a9 --enable-thumb --enable-neon --target-os=linux

# This is where FFMPEG headers live.
CXXFLAGS += -I$(PATH_STAGING)/include

.PHONY: all clean dist-clean rebuild

all: $(PACKAGE)
rebuild: clean $(PACKAGE)
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

# Using FFMPEG to demux, one of the ways to compile third-party library.
$(PATH_STAGING)/lib/libavformat.a:
	@echo "######################################################################"
	@echo "#    BUILDING FFMPEG"
	@echo "######################################################################"
ifeq (,$(wildcard $(FFMPEG_ARCHIVE)))
	wget https://ffmpeg.org/releases/ffmpeg-snapshot.tar.bz2
endif
ifeq (,$(wildcard ffmpeg))
	tar xjvf $(FFMPEG_ARCHIVE)
endif
	rm -rf $(PATH_STAGING)
	mkdir $(PATH_STAGING)
	cd ffmpeg && ./configure $(CONFIGURE_OPTIONS) && make -j`nproc` install

# Application to render video ES.
$(PATH_PACKAGE)/sample-video: $(PATH_STAGING)/lib/libavformat.a $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/sample-video"
	@echo "######################################################################"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)
	$(LD) $^ $(LDFLAGS) -lRokuIDKAV -lEGL -lGLESv2 -L$(PATH_STAGING)/lib -lavformat -lavcodec -lavutil -lswresample -lz -lssl -lcrypto -o $(PATH_PACKAGE)/sample-video
	$(STRIP) $(PATH_PACKAGE)/sample-video

# Populate staging directory with manifest and create binary for side-loading.
$(PACKAGE): $(PATH_PACKAGE)/sample-video
	@echo "######################################################################"
	@echo "#    PACKAGING $(PACKAGE)"
	@echo "######################################################################"
	cp -f assets/manifest $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)/source
	cp -f assets/main.brs $(PATH_PACKAGE)/source
	cp -f $(COMMON)/icon-246x140.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/icon-290x218.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/splash-720p.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/splash-480p.png $(PATH_PACKAGE)/source
	cp -a streams $(PATH_PACKAGE)
	chmod -R 755 $(PATH_PACKAGE)
	$(UTIL_MKSQUASHFS) $(PATH_PACKAGE) $(PACKAGE) -force-uid 499 -force-gid 499 -noappend -comp gzip

# Delete everything.
dist-clean: clean
	rm -rf $(PATH_STAGING)
	rm -rf $(PATH_FFMPEG)
	rm -f $(FFMPEG_ARCHIVE)

clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS)
	rm -rf $(PATH_PACKAGE)
