CC := gcc

EXEC := li3
CFLAGS := -std=c11 -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors

CFLAGS += $(shell pkg-config --cflags glib-2.0)
LIBS := $(shell pkg-config --libs glib-2.0) -lreadline

BUILD_TYPE ?= release
ifeq ($(BUILD_TYPE), debug)
    CFLAGS += -O0 -g -DDEBUG=1
    EXEC := $(EXEC)-debug
    BUILD_DIR_PATH := build/debug
    SRC_FOLDER := src
    CFLAGS += -Iinclude/
    SRC += $(wildcard $(SRC_FOLDER)/*.c $(SRC_FOLDER)/*/*.c)
else ifeq ($(BUILD_TYPE), release)
    CFLAGS += -g -O3 -flto -funroll-loops
    EXEC := $(EXEC)-release
    BUILD_DIR_PATH := build/release
    SRC_FOLDER := src
    CFLAGS += -Iinclude/
    SRC += $(wildcard $(SRC_FOLDER)/*.c $(SRC_FOLDER)/*/*.c)
else ifeq ($(BUILD_TYPE), test)
	CFLAGS += -O0 -g -DOMIT_BENCHMARK_LOGGING
	EXEC := $(EXEC)-test
	BUILD_DIR_PATH := build/test
	SRC += test/main.c $(filter-out src/main.c,$(wildcard src/*.c src/*/*.c))
	CFLAGS += -Iinclude/
else
	$(error Invalid build type: $(BUILD_TYPE))
endif

OBJ := $(addprefix $(BUILD_DIR_PATH)/, $(SRC:.c=.o))

WHITE := \033[0m
YELLOW := \033[1;33m

.PHONY: all
all: build-release copy-build-release-to-main-folder

build-debug:
	@$(MAKE) BUILD_TYPE=debug build

build-release:
	@$(MAKE) BUILD_TYPE=release build

build-test:
	@$(MAKE) BUILD_TYPE=test build

build: $(BUILD_DIR_PATH)/$(EXEC)

$(BUILD_DIR_PATH)/$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS) -lm $(LIBS)
	@printf "$(YELLOW)$(EXEC) $(WHITE)compiled.\n"

$(BUILD_DIR_PATH)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "Compiling $(YELLOW)$<$(WHITE)...\n"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf build
	rm -rf Resultados
	rm -f programa-principal
	rm -rf doxygen-output
	@$(MAKE) -C csv_generator clean

format:
	clang-format -i src/*.c include/*.h test/*.c

copy-build-release-to-main-folder: build-release
	cp $(BUILD_DIR_PATH)/$(EXEC) programa-principal

leaks: build-debug
ifeq ($(shell uname), Darwin)
	leaks --quiet --atExit -- ./build/debug/li3-debug datasets/data-regular datasets/data-regular/input2.txt
else
	valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible,reachable --error-exitcode=1 \
	./build/debug/li3-debug datasets/data-regular datasets/data-regular/input1.txt 
endif

test: build-test build-release
	@./build/test/li3-test

	@echo "# Start of end-to-end tests"
	@rm -rf Resultados
	@./build/release/li3-release datasets/data-regular datasets/data-regular/input1.txt > /dev/null
	@diff Resultados datasets/data-regular/expected-results-1
	@echo "ok /end-to-end/regular_1"

	@rm -rf Resultados
	@./build/release/li3-release datasets/data-regular datasets/data-regular/input2.txt > /dev/null
	@diff Resultados datasets/data-regular/expected-results-2
	@echo "ok /end-to-end/regular_2"

	@rm -rf Resultados
	@echo "# End of end-to-end tests"
	@echo "\nTests passed!"

compile-latex:
	cd Relatorio && pdflatex -interaction=nonstopmode -halt-on-error -file-line-error relatorio-fase1.tex && cp relatorio-fase1.pdf ../

generate-doxygen:
	doxygen doxyfile.cfg
	python3 -m coverxygen --xml-dir doxygen-output/xml --src-dir . --output doxygen-output/doc-coverage.info --kind enum,function,struct,union,typedef
	genhtml --no-function-coverage --no-branch-coverage doxygen-output/doc-coverage.info -o doxygen-output
	realpath doxygen-output/index.html | sed 's/^/file:\/\//'

generate-csv:
	@$(MAKE) -C csv_generator build
	@./csv_generator/csv_generator $(filter-out $@,$(MAKECMDGOALS))
