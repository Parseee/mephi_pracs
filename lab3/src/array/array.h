#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"
#include "../logger/logger.h"

#define DEBUG

typedef struct {
    int* data;
    size_t size;
} Array;

error_state Array_create(Array* array, size_t size);
error_state Array_insert(Array* array, int value, size_t index);
error_state Array_init(Array* array, size_t init_size, error_state (*input_handle)(int*, FILE*, size_t ));
error_state Array_remove(Array* array, size_t index);
error_state Array_func(Array* array);
error_state Array_print(Array* array);
error_state Array_kill(Array* array);

#endif /* ARRAY_H */