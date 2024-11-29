.PHONY: all clean

PROJECT_NAME = onegin

SRC_DIR = ./src
BUILD_DIR = ./build
CC = gcc-14

DEBUG_ = 1

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

SOURCES = main.c logger/logger.c text/text.c sort/sort.c
SOURCES_REL_PATH = $(patsubst %.c,$(SRC_DIR)/%.c,$(SOURCES))

DIRS = text logger sort
BUILD_DIRS = $(DIRS:%=$(BUILD_DIR)/%)

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

# -include $(patsubst %.o,%.d,$(OBJECTS_REL_PATH))

$(BUILD_DIRS):
	@mkdir -p $@

clean: clean_bin clean_obj clean_log

clean_bin:
	rm $(PROJECT_NAME).out

clean_log:
	rm -rf .log/*.log

clean_obj:
	rm -rf ./build
