CC := gcc

EXEC := li3
CFLAGS := -std=c11 -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors
CFLAGS += -Iinclude/

CFLAGS += $(shell pkg-config --cflags glib-2.0)
LIBS := $(shell pkg-config --libs glib-2.0)

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -O0 -g -DDEBUG=1
    EXEC := $(EXEC)-debug
    BUILD_DIR_PATH := build/debug
else
    CFLAGS += -O2 -flto
    EXEC := $(EXEC)-release
    BUILD_DIR_PATH := build/release
endif

BUILD_DIR := $(shell mkdir -p $(BUILD_DIR_PATH))

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=$(BUILD_DIR_PATH)/%.o)

WHITE := \033[0m
YELLOW := \033[1;33m

.PHONY: all
all: build-release copy-build-release-to-main-folder

build-debug:
	@$(MAKE) DEBUG=1 build

build-release:
	@$(MAKE) DEBUG=0 build

build: $(BUILD_DIR_PATH)/$(EXEC)

$(BUILD_DIR_PATH)/$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) -lm $(LIBS)
	@printf "$(YELLOW)$(EXEC) $(WHITE)compiled.\n"

$(BUILD_DIR_PATH)/%.o: src/%.c
	@printf "Compiling $(YELLOW)$<$(WHITE)...\n"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf build
	rm -rf Resultados
	rm -f programa-principal

format:
	clang-format -i src/*.c include/*.h

copy-build-release-to-main-folder: build-release
	cp $(BUILD_DIR_PATH)/$(EXEC) programa-principal

# TODO: add support for valgrind
leaks: build-debug
	@leaks --quiet --atExit -- ./build/debug/li3-debug dataset input.txt

test-results:
	@diff -yr Resultados/ Resultados-expected/ | grep -v "Only in Resultados-expected" || exit 0

compile-latex:
	cd Relatorio && pdflatex -interaction=nonstopmode -halt-on-error -file-line-error relatorio-fase1.tex && mv relatorio-fase1.pdf ../
