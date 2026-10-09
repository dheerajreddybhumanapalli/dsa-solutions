CXX       := g++
CXXFLAGS  := -Wall -Wextra -O2 -std=c++17
BUILD_DIR := build
ROOT_DIR  := $(CURDIR)

# Discover all nested directories containing init.cpp
INIT_SRCS := $(shell find . -mindepth 2 -type f -name init.cpp -not -path "*/.*" -not -path "./$(BUILD_DIR)/*" -not -path "./templates/*" -not -path "./docs/*")
DIRS      := $(sort $(patsubst %/init.cpp,%,$(patsubst ./%,%,$(INIT_SRCS))))

# Support shell tab-completions with trailing slash (e.g. dir/)
DIR_VARIANTS := $(DIRS) $(addsuffix /,$(DIRS))

RUN_TARGETS     := $(addsuffix /run,$(DIRS))
REBUILD_TARGETS := $(addsuffix /rebuild,$(DIRS))

# ----------------------------------------------------------------------
# Python: discover all nested directories containing init.py
# ----------------------------------------------------------------------
PYTHON := python3

PY_INIT_SRCS := $(shell find . -mindepth 2 -type f -name init.py -not -path "*/.*" -not -path "./$(BUILD_DIR)/*" -not -path "./templates/*" -not -path "./docs/*")
PY_DIRS      := $(sort $(patsubst %/init.py,%,$(patsubst ./%,%,$(PY_INIT_SRCS))))
PY_TARGETS   := $(addprefix py-,$(PY_DIRS))

.PHONY: all py-all clean $(DIR_VARIANTS) $(RUN_TARGETS) $(REBUILD_TARGETS) $(PY_TARGETS)
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

# Normalize path variants (e.g. make leetcode/1401/)
$(addsuffix /,$(DIRS)): %/: %

# Compile <dir>/init.cpp directly into the executable
# -I. lets init.cpp find runner.h in the root directory
# -I$* lets init.cpp find solution.cpp in the problem's own directory
# Makefile is a prereq so flag changes trigger a rebuild
$(BUILD_DIR)/%/app: %/init.cpp %/solution.cpp runner.h Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I. -I$* $< -o $@
	@echo "Built $@"

# Run <dir>'s test suite (builds first if stale)
# e.g. make leetcode/3550/run
$(RUN_TARGETS): %/run: $(BUILD_DIR)/%/app
	(cd $* && "$(ROOT_DIR)/$(BUILD_DIR)/$*/app")

# Force rebuild <dir> (shows warnings again)
# e.g. make leetcode/3550/rebuild
$(REBUILD_TARGETS): %/rebuild:
	rm -rf $(BUILD_DIR)/$*/app
	@$(MAKE) $*

# ----------------------------------------------------------------------
# Target: make py-all
# Runs the Python test suite for every problem that ships an init.py
# ----------------------------------------------------------------------
py-all: $(PY_TARGETS)

# ----------------------------------------------------------------------
# Target: make py-<dir>  (e.g. make py-leetcode/20)
# Runs <dir>/init.py against <dir>/testcases.txt
# PYTHONPATH=. lets init.py find runner.py in the root (mirrors -I. for C++)
# ----------------------------------------------------------------------
$(PY_TARGETS): py-%:
	PYTHONPATH="$(ROOT_DIR)" $(PYTHON) "$*/init.py" "$*/testcases.txt"

clean:
	rm -rf $(BUILD_DIR)
	find . -type d -name __pycache__ -prune -exec rm -rf {} +