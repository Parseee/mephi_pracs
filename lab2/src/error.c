#include "error.h"
#include <stdio.h>
#include <stdlib.h>

#define report_error(err)                                                \
    fprintf(stderr, err" in file %s on line %d\n", __FILE__, __LINE__); \
    return EXIT_FAILURE;

int handle_error(ERROR_CODE const error) {
    switch (error) {
        case CODE_OK:
            report_error("OK");            
            break;

        case INTEGER_OVERFLOW_ERROR:
            report_error("Integer overflow");
            break;

        case LOGIC_ERROR:
            report_error("Logic error");
            break;

        case INPUT_ERROR:
            report_error("Input reading error");            
            break;

        default:
            report_error("Something happened idk\n");
            break;
    }
}

