# Copyright 2021 Roku, Inc.  All rights reserved.
PATH_BUILD ?= build
PATH_SOURCE ?= sources
PATH_PACKAGE ?= package
PATH_MANIFEST ?= assets/manifest
PATH_ASSETS ?= assets
PATH_COMPONENTS ?= components
USE_DEFAULT_ASSETS ?= yes
PACKAGE ?= idk-app.squashfs.bin
COMMON ?= ../common
VPATH := $(PATH_SOURCE)

COMPONENTS := $(wildcard $(PATH_COMPONENTS)/*)
ASSETS := $(wildcard $(PATH_ASSETS)/*)
C_SRCS := $(wildcard $(PATH_SOURCE)/*.c)
CXX_SRCS := $(wildcard $(PATH_SOURCE)/*.cpp)
CC_SRCS := $(wildcard $(PATH_SOURCE)/*.cc)
OBJECTS := $(C_SRCS:%.c=$(PATH_BUILD)/%.c.o)
OBJECTS += $(CXX_SRCS:%.cpp=$(PATH_BUILD)/%.cpp.o)
OBJECTS += $(CC_SRCS:%.cc=$(PATH_BUILD)/%.cc.o)
DEPENDS := $(OBJECTS:%.o=%.d)

.PHONY: env-check all clean repackage package-clean
all: env-check $(PACKAGE)
repackage: package-clean $(PACKAGE)
package-clean:
	rm -f $(PACKAGE)
	rm -rf $(PATH_PACKAGE)
clean:
	rm -f $(PACKAGE)
	rm -f $(OBJECTS) $(DEPENDS)
	rm -rf $(PATH_BUILD)
	rm -rf $(PATH_PACKAGE)
env-check:
ifeq (, $(wildcard $(CXX)))
	$(error CXX is not set, did you forget to include Roku4.mk?)
endif
ifndef BINARY
	$(error Must define BINARY, name of your application.)
endif
ifndef PATH_MANIFEST
	$(error Must define PATH_MANIFEST, file containing information about your application.)
endif

$(PATH_PACKAGE)/$(BINARY): $(OBJECTS)
	@echo "######################################################################"
	@echo "#    BUILDING $(PATH_PACKAGE)/$(BINARY)"
	@echo "######################################################################"
	@echo "SOURCES: $(C_SRCS)$(CC_SRCS)$(CXX_SRCS)"
	rm -rf $(PATH_PACKAGE)
	mkdir -p $(PATH_PACKAGE)
	$(LD) $(LDFLAGS) -lz $^ -o $(PATH_PACKAGE)/$(BINARY)
	$(STRIP) $(PATH_PACKAGE)/$(BINARY)

$(PACKAGE): $(PATH_PACKAGE)/$(BINARY) $(ASSETS) $(COMPONENTS)
	@echo "######################################################################"
	@echo "#    PACKAGING $(PACKAGE)"
	@echo "######################################################################"
	rm -f $(PACKAGE)
	rm -rf $(PATH_PACKAGE)/source
	cp -a $(PATH_ASSETS) $(PATH_PACKAGE)/source
	rm -f `find $(PATH_PACKAGE)/source -name manifest`
	cp -f $(PATH_MANIFEST) $(PATH_PACKAGE)
ifeq ($(USE_DEFAULT_ASSETS),yes)
	cp -f $(COMMON)/icon-246x140.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/icon-290x218.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/splash-720p.png $(PATH_PACKAGE)/source
	cp -f $(COMMON)/splash-480p.png $(PATH_PACKAGE)/source
endif
ifneq (, $(wildcard $(PATH_COMPONENTS)))
	rm -rf $(PACK_PACKAGE)/components
	cp -a $(PATH_COMPONENTS) $(PATH_PACKAGE)/
endif
	chmod -R 755 $(PATH_PACKAGE)
	$(UTIL_MKSQUASHFS) $(PATH_PACKAGE) $(PACKAGE) -force-uid 499 -force-gid 499 -noappend -comp gzip

$(PATH_BUILD)/$(PATH_SOURCE)/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(PATH_BUILD)/$(PATH_SOURCE)/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(PATH_BUILD)/$(PATH_SOURCE)/%.cc.o: %.cc
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPENDS)
