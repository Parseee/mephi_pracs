#pragma once

#include "../db/db.h"
#include <assert.h>
#include <stdio.h>

typedef enum IO_state { IO_OK,
    IO_INPUT_ERROR,
    IO_OUTPUT_ERROR,
    IO_INTERNAL_ERROR } IO_state;

IO_state IO_terminal_input(DB* db);
IO_state IO_text_input(DB* db, const char* const filename);
IO_state IO_binary_input(DB* db, const char* const filename);

IO_state IO_terminal_output(DB* db);
IO_state IO_text_output(DB* db, const char* const filename);
IO_state IO_binary_output(DB* db, const char* const filename);

// IO_state construct_item(Item** item, char* str);