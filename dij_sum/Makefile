.PHONY: all clean

PROJECT_NAME = dijit_sum

SRC_DIR = ./src
BUILD_DIR = ./build
CC = gcc

DEBUG_ = 1

CFLAGS = -O2 -Wall -Wextra  \
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


SOURCES = main.c

SOURCES_REL_PATH = $(SOURCES:%=$(SRC_DIR)/%)
OBJECTS_REL_PATH = $(SOURCES:%.c=$(BUILD_DIR)/%.c.o)

all: build run

build: $(PROJECT_NAME).out

run:
	./$(PROJECT_NAME).out


$(PROJECT_NAME).out: $(OBJECTS_REL_PATH)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJECTS_REL_PATH): $(SOURCES_REL_PATH) | $(BUILD_DIRS)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIRS):
	@mkdir $(BUILD_DIR)

clean: clean_bin clean_obj

clean_bin:
	rm $(PROJECT_NAME).out

clean_obj:
	rm -rf ./build
