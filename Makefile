.SUFFIXES:

BUILD_TYPE       ?= debug

CXX              := gcc
CXXFLAGS_DEBUG   := -std=c23 -Wall -Wextra -pedantic -g -O0 -MMD -MP
CXXFLAGS_RELEASE := -std=c23 -Wall -Wextra -pedantic -O3 -DNDEBUG -march=native -flto -MMD -MP

SRC_DIR          := src
BASE_BUILD_DIR   := build
BASE_BIN_DIR     := bin
BUILD_DIR        := $(BASE_BUILD_DIR)/$(BUILD_TYPE)
BIN_DIR          := $(BASE_BIN_DIR)/$(BUILD_TYPE)

EXEC             := poopcast
TARGET           := $(BIN_DIR)/$(EXEC)

SRCS             := $(shell find $(SRC_DIR) -name "*.c")
OBJS             := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS             := $(OBJS:.o=.d)

ifeq ($(BUILD_TYPE),debug)
  CXXFLAGS := $(CXXFLAGS_DEBUG)
else ifeq ($(BUILD_TYPE),release)
  CXXFLAGS := $(CXXFLAGS_RELEASE)
else
  $(error Unknown BUILD_TYPE "$(BUILD_TYPE)"; must be "debug" or "release")
endif

.PHONY: build clean debug release format

build: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BASE_BUILD_DIR) $(BASE_BIN_DIR)

debug:
	$(MAKE) BUILD_TYPE=debug

release:
	$(MAKE) BUILD_TYPE=release

format:
	find $(SRC_DIR) -iname '*.h' -o -iname '*.c' | xargs clang-format -i --verbose

run: release
	$(BASE_BIN_DIR)/release/$(EXEC)

