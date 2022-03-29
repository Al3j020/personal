# Copyright 2021 Roku, Inc.  All rights reserved.
BUILD_DIRS := $(shell  find . -maxdepth 1 -type d -not -path "." -not -name "common" -printf "%f\n")

.PHONY: all clean force

all: $(BUILD_DIRS)

clean:
	@$(foreach d, $(BUILD_DIRS),\
	echo " "; \
	echo " "; \
	echo "***************************************"; \
	echo "Cleaning sample $(d)"; \
	echo "***************************************"; \
	echo " "; \
	echo " "; \
	make -C $(d) --no-print-directory clean;) \

$(BUILD_DIRS): force
	@echo " "
	@echo " "
	@echo "***************************************"
	@echo "Building sample $@"
	@echo "***************************************"
	@echo " "
	@echo " "
	@- $(MAKE) -C $@ --no-print-directory 

force: ;
