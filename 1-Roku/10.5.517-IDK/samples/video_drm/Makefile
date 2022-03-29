# Copyright 2021 Roku, Inc.  All rights reserved.
#
# This Makefile is different from the other samples to show one way to fetch and
# build a library like Bento4.
COMMON := ../common
include $(COMMON)/Roku4.mk

# Define target binaries we want to build.
BENTO_ARCHIVE := Bento4-SRC-1-6-0-638.zip
PATH_BENTO := bento
PATH_STAGING := $(abspath staging)
PATH_PACKAGE := package
PACKAGE := ./idk_sample_video_drm.squashfs.bin
OBJECTS := main.o buffer.o screen.o license.o bento4.o
INSTALL_PREFIX = $(PATH_STAGING)

# This is where Bento4 headers live.
BHEADER := $(PATH_BENTO)/Source/C++
CXXFLAGS += -I$(BHEADER)/Core -I$(BHEADER)/Codecs -I$(BHEADER)/Crypto -I$(BHEADER)/MetaData

.PHONY: all clean dist-clean rebuild

all: $(PACKAGE)
rebuild: clean $(PACKAGE)
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

# Using Bento4 to demux, compile the third-party library.
$(PATH_STAGING)/libap4.a:
	@echo "######################################################################"
	@echo "#    BUILDING BENTO4"
	@echo "######################################################################"
ifeq (,$(wildcard $(BENTO_ARCHIVE)))
	wget https://www.bok.net/Bento4/source/Bento4-SRC-1-6-0-638.zip
endif
ifeq (,$(wildcard $(PATH_BENTO)))
	unzip $(BENTO_ARCHIVE) -d $(PATH_BENTO)
endif
	rm -rf $(PATH_STAGING)
	mkdir -p $(PATH_STAGING) && cd $(PATH_STAGING) && CXX=$(CXX) CC=$(CC) LD=$(LD) cmake -DBUILD_APPS=OFF ../$(PATH_BENTO) && make

# Application to render video ES.
$(PATH_PACKAGE)/sample-video: $(PATH_STAGING)/libap4.a $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/sample-video"
	@echo "######################################################################"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)
	$(LD) $^ $(LDFLAGS) -lRokuIDKAV -lRokuIDKDrm -lEGL -lGLESv2 -lcurl -L$(PATH_STAGING) -lap4 -o $(PATH_PACKAGE)/sample-video
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
	rm -rf $(PATH_BENTO)
	rm -f $(BENTO_ARCHIVE)

clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS)
	rm -rf $(PATH_PACKAGE)
