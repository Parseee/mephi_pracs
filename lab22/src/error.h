#pragma once

#include <assert.h>
#include <stdio.h>

typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR, INPUT_ERROR, INVALID_INPUT_ERROR, OTHER_ERROR} ERROR_CODE;


int handle_error(ERROR_CODE const error);
ERROR_CODE handle_input_floating(char const * const, float * const );
ERROR_CODE handle_input_int(char const * const, int64_t * const);
