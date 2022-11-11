CC := gcc

EXEC := li3
FLAGS := -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds 	
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g -DDEBUG
else
    FLAGS += -O2
endif
LIBS := -lm `pkg-config --libs glib-2.0`
PKG_CONFIG := `pkg-config --cflags glib-2.0`
INC := -I include/

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)

BUILD_DIR := $(shell mkdir -p build)
OUT_DIR := $(shell mkdir -p out)
INCLUDE_DIR := $(wildcard include/*.h)

.PHONY: all
all: build/$(EXEC)

build/$(EXEC): $(OBJ)
	$(CC) $(FLAGS) $^ ${PKG_CONFIG} $(LIBS) -o $@

build/%.o: src/%.c
	$(CC) $(FLAGS) -c $< ${PKG_CONFIG} $(LIBS) $(INC) -o $@

.PHONY: clear
clear:
	rm -rf build/*
