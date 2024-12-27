.PHONY: all clean

PROJECT_NAME = lab5

SRC_DIR = ./src
BUILD_DIR = ./build
CC = gcc-13.2
UNAME = $(shell uname -s)
ifeq ($(UNAME), Darwin)
	CC = gcc-14
endif

CFLAGS =-Wall -Wextra -Waggressive-loop-optimizations -g \
		-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
		-Wconversion -Wempty-body -Wfloat-equal \
		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
		-Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self \
		-Wredundant-decls -Wshadow -Wsign-conversion \
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
		-Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand \
		-Wundef -Wunreachable-code -Wunused -Wvariadic-macros \
		-Wno-missing-field-initializers -Wno-narrowing -Wno-varargs \
		-Wstack-protector -fcheck-new -fstack-protector -fstrict-overflow \
		-fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 \
		-fPIE -Werror=vla

SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,$\
		integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,$\
		shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

SOURCES = main.c db/db.c io_kit/io_kit.c sort/sort.c
SOURCES_REL_PATH = $(patsubst %.c,$(SRC_DIR)/%.c,$(SOURCES))

DIRS = db io_kit sort
BUILD_DIRS = $(DIRS:%=$(BUILD_DIR)/%)

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
OBJECTS_REL_PATH = $(patsubst %.o,$(BUILD_DIR)/%.o,$(OBJECTS))

all: build run

build: $(PROJECT_NAME).out

run:
	@./$(PROJECT_NAME).out

$(PROJECT_NAME).out: $(OBJECTS_REL_PATH)
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIRS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LIBS)

$(BUILD_DIRS):
	@mkdir -p $@

clean: clean_bin clean_obj clean_log

clean_bin:
	rm $(PROJECT_NAME).out

clean_log:
	rm -rf .log/*.log

clean_obj:
	rm -rf ./build
	rm -rf *ans*

rebuild: clean build
