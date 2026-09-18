CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
BUILD_DIR := build

# All optimal solutions: leetcode/*/solution.cpp
SOLUTIONS := $(wildcard leetcode/*/solution.cpp)

.PHONY: all clean help
all: $(SOLUTIONS)
	@mkdir -p $(BUILD_DIR)
	@for f in $(SOLUTIONS); do \
		id=$$(echo $$f | cut -d/ -f2 | cut -d- -f1); \
		$(CXX) $(CXXFLAGS) $$f -o $(BUILD_DIR)/$$id || exit 1; \
		echo "built $(BUILD_DIR)/$$id"; \
	done

# make 1520 -> builds leetcode/1520-*/solution.cpp to build/1520
%: leetcode/%-*/solution.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $(BUILD_DIR)/$*
	@echo "built $(BUILD_DIR)/$* -- run with ./$(BUILD_DIR)/$*"

clean:
	rm -rf $(BUILD_DIR)/*

help:
	@echo "Targets: make <problem-id> (e.g. make 1520), make all, make clean"
