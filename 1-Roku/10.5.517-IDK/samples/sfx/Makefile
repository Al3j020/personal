# Copyright 2021 Roku, Inc.  All rights reserved.

# First define directory where rules.mk and Roku4.mk live.
COMMON = ../common

# Include platform default defines for toolchain, CFLAGS, LDFLAGS, etc.
include $(COMMON)/Roku4.mk

# Name of executable.
BINARY = sample-sfx

# End result of packaging.  Side-load this binary directly via the developer
# installer interface.
PACKAGE = ./idk_sample_sfx.squashfs.bin

# Directory where launch BrightScript and additional assets live.  Everything
# is copied, so assets -> $(PATH_PACKAGE)/source.  If you have customized icons
# and splash screens different from the ones provided in ../common/, you can
# change USE_DEFAULT_ASSETS to "no", place those images in the asset directory
# and change references to them in your manifest file.
#PATH_ASSETS = assets
#USE_DEFAULT_ASSETS = yes

# Directory where manifest file lives.  The manifest file specifies name,
# channel icons, and splash screen.
PATH_MANIFEST = assets/manifest

# Additional library to link.  Note it uses +=, and should only be set after
# including Roku4.mk.
LDFLAGS += -lRokuIDKSfx

# Use above variables and populate rest of Makefile with the following rules:
#
#       all: Will build $(PACKAGE).  Incremental build will also work.
#     clean: Delete everything, the way this sample directory came.
# repackage: Files in your assets/ changed, copy them again and update package.
include $(COMMON)/rules.mk
