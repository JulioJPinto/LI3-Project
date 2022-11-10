CC := gcc

EXEC := li3
CFLAGS := -std=c17 -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -lm
LIBS := $(shell pkg-config --libs glib-2.0)
CFLAGS += $(shell pkg-config --cflags glib-2.0)
CFLAGS += -Iinclude/

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g -DDEBUG
    EXEC := $(EXEC)-debug
else
    FLAGS += -O2
    EXEC := $(EXEC)-release
endif

BUILD_BASE_DIR := build
BUILD_DIR_PATH :=
ifeq ($(DEBUG), 1)
    BUILD_DIR_PATH := $(BUILD_BASE_DIR)/debug
else
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
	@printf "$(YELLOW)$(EXEC) $(WHITE)compiled."

$(BUILD_DIR_PATH)/$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(BUILD_DIR_PATH)/%.o: src/%.c
	@printf "Compiling $(YELLOW)$< $(WHITE)...\n"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clear
clear:
	rm -rf build