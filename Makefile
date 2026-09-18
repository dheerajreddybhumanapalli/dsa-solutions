CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
BUILD_DIR := build

# All optimal solutions: leetcode/*/solution.cpp
SOLUTIONS := $(wildcard leetcode/*/solution.cpp)

.PHONY: all clean help
all:
	@mkdir -p $(BUILD_DIR)
	@for f in $(SOLUTIONS); do \
		id=$$(echo $$f | cut -d/ -f2 | cut -d- -f1); \
		$(CXX) $(CXXFLAGS) -DLOCAL $$f -o $(BUILD_DIR)/$$id || exit 1; \
		echo "built $(BUILD_DIR)/$$id"; \
	done

%:
	@mkdir -p $(BUILD_DIR)
	@f=$$(ls -d leetcode/$*-*/solution.cpp leetcode/$*/solution.cpp 2>/dev/null | head -n 1); \
	if [ -z "$$f" ]; then echo "no solution for id $* (looked in leetcode/$*/ and leetcode/$*-*/)"; exit 1; fi; \
	$(CXX) $(CXXFLAGS) -DLOCAL $$f -o $(BUILD_DIR)/$* || exit 1; \
	echo "built $(BUILD_DIR)/$* -- run with ./$(BUILD_DIR)/$*"

clean:
	rm -rf $(BUILD_DIR)/*

help:
	@echo "Targets: make <problem-id> (e.g. make 1520), make all, make clean"
