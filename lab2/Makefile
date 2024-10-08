.PHONY: all clean

PROJECT_NAME = lab2

SRC_DIR = ./src
BUILD_DIR = ./build
CC = gcc-14

DEBUG_ = 1

CFLAGS = -O2 -Wall -Wextra -g \
		-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
		-Wconversion -Wempty-body -Wfloat-equal \
		-Wformat-nonliteral -Wformat-security -Wformat=2 -Winline \
		-Wpacked -Wpointer-arith -Winit-self \
		-Wredundant-decls -Wshadow -Wsign-conversion \
		-Wstrict-overflow=2 \
		-Wswitch-default -Wswitch-enum \
		-Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
		-Wno-missing-field-initializers -Wno-narrowing -Wno-varargs \
		-Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow \
		-fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-protector -Werror=vla \
		-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
		integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
		shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


SOURCES = approx.c error.c
SOURCES_REL_PATH = $(patsubst %,$(SRC_DIR)/%,$(SOURCES))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS_REL_PATH = $(patsubst %,$(BUILD_DIR)/%,$(OBJECTS))
EXECS = $(patsubst %.c,%.out,$(SOURCES))

all: $(EXECS)

$(EXECS): $(OBJECTS_REL_PATH)
	@$(CC) $(CFLAGS) $< -o $@

$(OBJECTS_REL_PATH): $(SOURCES_REL_PATH)
	@$(CC) $(CFLAGS) -c $< -o $@

clean: clean_bin clean_obj

clean_bin:
	rm $(EXECS)

clean_obj:
	rm -rf ./build
