#include <assert.h>
#include <complex.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "error.h"
#include "logger/logger.h"

const char* handle_error(ERROR_CODE const error)
{
    switch (error) {
    case CODE_OK:
        return "OK";
        break;

    case INTEGER_OVERFLOW_ERROR:
        return "Integer overflow";
        break;

    case LOGIC_ERROR:
        return ("Logic error");
        break;

    case INPUT_ERROR:
        return ("Input reading error");
        break;

    case INVALID_INPUT_ERROR:
        return ("Invalid input data");
        break;

    default:
        return ("Something happened idk");
        break;
    }
}

static int check_EOF(char const* const str)
{ // what to return
    int idx = 0;
    while (str[idx] != '\0') {
        if (str[idx] == EOF) {
            return EOF;
        }
        ++idx;
    }
    return 0;
}

ERROR_CODE handle_input_complex(char const* const input, double complex* const x)
{
    lassert(input, "input string is null");
    lassert(x, "result is null");

    if (feof(stdin)) {
        clearerr(stdin);
        report_error(INPUT_ERROR);
    }

    char* start_ptr = (char*)((void*)input);
    char* end_ptr = NULL;
    double re = strtod(start_ptr, &end_ptr);

    start_ptr = end_ptr;
    double im = strtod(start_ptr, &end_ptr);

    *x = CMPLX(re, im);

    if ((*x == HUGE_VALF) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return INVALID_INPUT_ERROR;
    }
    return CODE_OK;
}

ERROR_CODE handle_input_int(char const* const input, int64_t* const x)
{
    lassert(input, "input string is null");
    lassert(x, "result is null");

    if (feof(stdin)) {
        clearerr(stdin);
        return INPUT_ERROR;
    }

    char* end_ptr;

    *x = strtoll(input, &end_ptr, 10);

    if ((*x == LLONG_MIN || *x == LLONG_MAX) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return INVALID_INPUT_ERROR;
    }

    return CODE_OK;
}

ERROR_CODE handle_input_floating(char const* const input, double* const x)
{
    lassert(input, "input string is null");
    lassert(x, "result is null");

    if (feof(stdin)) {
        clearerr(stdin);
        report_error(INPUT_ERROR);
    }

    char* end_ptr;

    *x = strtof(input, &end_ptr);

    if ((*x == HUGE_VALF) && errno == ERANGE) {
        return INTEGER_OVERFLOW_ERROR;
    }

    if (errno == EINVAL) {
        return INVALID_INPUT_ERROR;
    }

    return CODE_OK;
}