CC := gcc

EXEC := li3
CFLAGS := -std=c17 -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds
CFLAGS += -Iinclude/

CFLAGS += $(shell pkg-config --cflags glib-2.0)
LIBS := $(shell pkg-config --libs glib-2.0)

BUILD_BASE_DIR := build
BUILD_DIR_PATH :=

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g -DDEBUG
    EXEC := $(EXEC)-debug
    BUILD_DIR_PATH := $(BUILD_BASE_DIR)/debug
else
    FLAGS += -O2
    EXEC := $(EXEC)-release
    BUILD_DIR_PATH := $(BUILD_BASE_DIR)/release
endif

BUILD_DIR := $(shell mkdir -p $(BUILD_DIR_PATH))

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=$(BUILD_DIR_PATH)/%.o)

WHITE := \033[0m
YELLOW := \033[1;33m

.PHONY: all
all: build

build: $(BUILD_DIR_PATH)/$(EXEC)

$(BUILD_DIR_PATH)/$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) -lm $(LIBS)
	@printf "$(YELLOW)$(EXEC) $(WHITE)compiled."

$(BUILD_DIR_PATH)/%.o: src/%.c
	@printf "Compiling $(YELLOW)$<$(WHITE)...\n"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf build