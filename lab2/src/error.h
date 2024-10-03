#pragma once

#include <assert.h>
#include <stdio.h>

typedef enum { CODE_OK = 0, INTEGER_OVERFLOW_ERROR, LOGIC_ERROR, INPUT_ERROR, GOVNO_ERROR} ERROR_CODE;


int handle_error(ERROR_CODE const error); 
