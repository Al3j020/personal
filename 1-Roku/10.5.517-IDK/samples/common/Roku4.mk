# Copyright 2021 Roku, Inc.  All rights reserved.
PATH_IDK = $(abspath ../../platforms/Roku4/)
PATH_LIB = $(PATH_IDK)/lib
PATH_USR = $(PATH_IDK)/usr
PATH_TOOLCHAIN = $(PATH_IDK)/toolchain

CROSS_COMPILE = $(PATH_TOOLCHAIN)/bin/arm-roku-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib

COMMON_FLAGS = -mcpu=cortex-a9 -mthumb \
	-fvisibility=hidden -fdata-sections -ffunction-sections \
	-Wall -Wno-parentheses -U_FORTIFY_SOURCE \
	-I$(PATH_IDK)/include -I$(PATH_USR)/include

CFLAGS = -std=c99 $(COMMON_FLAGS)
CXXFLAGS = -std=c++17 -fvisibility-inlines-hidden $(COMMON_FLAGS)

PATH_LIBS = \
	-L $(PATH_LIB) -L$(PATH_USR)/lib

LDFLAGS = -Wl,--gc-sections -Wl,--copy-dt-needed-entries \
	-Wl,--allow-shlib-undefined -Wl,--no-as-needed \
	$(PATH_LIBS) -lc -lpthread -lrt -ldl -lstdc++

UTIL_MKSQUASHFS = $(abspath ../../bin/mksquashfs_roku)
UTIL_PAXCTL = $(abspath ../../bin/paxctl)
