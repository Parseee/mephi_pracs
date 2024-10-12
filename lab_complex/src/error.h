#pragma once

#include <assert.h>
#include <complex.h>
#include <stdio.h>

#include "logger/logger.h"

typedef enum { CODE_OK = 0,
    INTEGER_OVERFLOW_ERROR,
    LOGIC_ERROR,
    INPUT_ERROR,
    INVALID_INPUT_ERROR,
    OTHER_ERROR } ERROR_CODE;

#define report_error(err)                                                               \
    fprintf(stderr, handle_error(err), " in file %s on line %d\n", __FILE__, __LINE__); \
    return err;

const char* handle_error(ERROR_CODE const error);
ERROR_CODE handle_input_floating(char const* const input, double* const x);
ERROR_CODE handle_input_complex(char const* const input, double complex* const);
ERROR_CODE handle_input_int(char const* const, int64_t* const);
