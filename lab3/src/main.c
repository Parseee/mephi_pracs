#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger/logger.h"
#include "array/array.h"
#include "exec/exec.h"



int main(void)
{
    log_create();

    Array array = {.data = NULL, .size = 0};

    bool quit = false;
    while (!quit) {
        if (exec(&array)) {
            quit = true;
        }
    }

    log_destruct();

    return EXIT_SUCCESS;
}