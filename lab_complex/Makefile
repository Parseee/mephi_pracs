.PHONY: all clean

PROJECT_NAME = complex_func

SRC_DIR = ./src
BUILD_DIR = ./build
CC = gcc

DEBUG_ = 1

CFLAGS = -O2 -Wall -Wextra -g -std=c11\
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
		-fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-protector -Werror=vla
		
#-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
		integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
		shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SOURCES = main.c error.c logger/logger.c
SOURCES_REL_PATH = $(patsubst %.c,$(SRC_DIR)/%.c,$(SOURCES))

DIRS = logger
BUILD_DIRS = $(patsubst %,$(BUILD_DIR)/%,$(DIRS))

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS_REL_PATH = $(patsubst %.o,$(BUILD_DIR)/%.o,$(OBJECTS))

all: build run

build: $(PROJECT_NAME).out

run:
	@./$(PROJECT_NAME).out

$(PROJECT_NAME).out: $(OBJECTS_REL_PATH)
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIRS)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIRS):
	@mkdir -p $@

clean: clean_bin clean_obj

clean_bin:
	rm $(EXECS)

clean_obj:
	rm -rf ./build
