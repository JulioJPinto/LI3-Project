CC := gcc
CFLAGS := -g $(shell pkg-config --cflags glib-2.0)

INCLUDES := -I/usr/include/glib-2.0 -Iinclude
LIBS := $(shell pkg-config --libs glib-2.0)
PKG_CONFIG := $(shell pkg-config --cflags glib-2.0)

OBJ := build/main.o build/user.o build/parser.o build/ride.o build/driver.o 

.PHONY: main

main: ${OBJ}
	@${CC} ${CFLAGS} -o $@ ${INCLUDES} ${PKG_CONFIG} $^ ${LIBS} ; echo "Finished Successufly $@"

clean: 
	@rm build/*.o -f main
	

build/%.o: %.c
	@$(CC) $(FLAGS) -c $< ${PKG_CONFIG} ${LIBS} ${INC} -o $@ ; echo "Finished Successufly $@" 