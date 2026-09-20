CXX       := g++
CXXFLAGS  := -Wall -std=c++17
BUILD_DIR := build

# Discover all nested directories containing init.cpp
INIT_SRCS := $(shell find . -mindepth 2 -type f -name init.cpp -not -path "*/.*" -not -path "./$(BUILD_DIR)/*")
DIRS      := $(sort $(patsubst %/init.cpp,%,$(patsubst ./%,%,$(INIT_SRCS))))

.PHONY: all clean $(DIRS) $(addsuffix /,$(DIRS))
.DEFAULT_GOAL := all

# ----------------------------------------------------------------------
# Target: make all
# ----------------------------------------------------------------------
all: $(addprefix $(BUILD_DIR)/,$(addsuffix /app,$(DIRS)))
	@for app in $^; do \
		echo "=== Running $$app ==="; \
		./$$app || exit 1; \
	done

# ----------------------------------------------------------------------
# Targets: make <dir_path> and make <dir_path>/
# ----------------------------------------------------------------------
# Maps "make leetcode/1401" to the app binary
$(DIRS): %: $(BUILD_DIR)/%/app

# Maps "make leetcode/1401/" (tab-completion) to the app binary
$(addsuffix /,$(DIRS)): %/: $(BUILD_DIR)/%/app

# ----------------------------------------------------------------------
# Compilation Step
# ----------------------------------------------------------------------
# Because main.cpp #includes init.cpp, we only need to compile main.cpp.
# init.cpp is added as a prerequisite so Make knows to rebuild if it changes.
$(BUILD_DIR)/%/app: main.cpp %/init.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -DINIT_DIR='"$*"' -I$* $< -o $@
	@echo "Built $@"

clean:
	rm -rf $(BUILD_DIR)