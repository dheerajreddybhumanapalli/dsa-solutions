CXX       := g++
CXXFLAGS  := -Wall -std=c++17
BUILD_DIR := build
ROOT_DIR  := $(CURDIR)


# Discover all nested directories containing init.cpp
INIT_SRCS := $(shell find . -mindepth 2 -type f -name init.cpp -not -path "*/.*" -not -path "./$(BUILD_DIR)/*")
DIRS      := $(sort $(patsubst %/init.cpp,%,$(patsubst ./%,%,$(INIT_SRCS))))

# Support shell tab-completions (e.g. ./dir, dir/, ./dir/)
DIR_VARIANTS := $(DIRS) $(addsuffix /,$(DIRS)) $(addprefix ./,$(DIRS)) $(addprefix ./,$(addsuffix /,$(DIRS)))

.PHONY: all clean $(DIR_VARIANTS)
.DEFAULT_GOAL := all

# ----------------------------------------------------------------------
# Target: make all
# Builds every problem and executes its test suite
# ----------------------------------------------------------------------
all: $(DIRS)
	@for d in $(DIRS); do \
		echo "=== Running $$d ==="; \
		(cd "$$d" && "$(ROOT_DIR)/$(BUILD_DIR)/$$d/app") || exit 1; \
		echo ""; \
	done

# ----------------------------------------------------------------------
# Target: make <dir_path> (e.g. make leetcode/1401)
# ----------------------------------------------------------------------
$(DIRS): %: $(BUILD_DIR)/%/app

# Normalize path variants
$(addsuffix /,$(DIRS)): %/: %
$(addprefix ./,$(DIRS)): ./%: %
$(addprefix ./,$(addsuffix /,$(DIRS))): ./%/: %

# Compile <dir>/init.cpp directly into the executable
# -I. lets init.cpp find runner.h in the root directory
# -I$* lets init.cpp find solution.cpp in the problem's own directory
$(BUILD_DIR)/%/app: %/init.cpp %/solution.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I. -I$* $< -o $@
	@echo "Built $@"

clean:
	rm -rf $(BUILD_DIR)