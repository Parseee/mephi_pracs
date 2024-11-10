#include "array.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX(x, y) x < y ? y : x

error_state Array_insert(Array* array, int value, size_t index)
{
    lassert(array, "");

    if (index >= array->size) {
        index = array->size;
    }

    int* new_data = NULL;
    if (!(new_data = realloc(array->data, array->size + 1))) {
        free(array->data);
        array->size = 0;
        report_error(ALLOC_ERROR, "");
    }
    array->data = new_data;
    new_data = NULL;
    array->size += 1;

    for (size_t i = array->size - 1; i > index; --i) {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = value;

    return OK;
}

error_state Array_remove(Array* array, size_t index)
{
    lassert(array, "");

    if ((size_t)0 > index || index >= array->size) {
        report_error(LOGIC_ERROR, "index out of range");
    }

    for (size_t i = index; i < array->size - 1; ++i) {
        array->data[i] = array->data[i + 1];
    }

    int* new_data = NULL;
    if (!(new_data = realloc(array->data, array->size - 1))) {
        free(array->data);
        array->size = 0;
        report_error(ALLOC_ERROR, "");
    }

    array->data = new_data;
    array->size -= 1;

    return OK;
}

error_state Array_kill(Array* array)
{
    lassert(array, "");
    if (!array->data) {
        report_error(LOGIC_ERROR, "not created");
    }

    free(array->data);
    array->data = NULL;
    array->size = 0;

    return (!array->data) ? OK : ALLOC_ERROR;
}

error_state Array_print(Array* array)
{
    lassert(array, "");

    if (!array->data) {
        report_error(LOGIC_ERROR, "not created");
    }

    for (size_t i = 0; i < array->size; ++i) {
        printf("%d ", array->data[i]);
    }
    printf("\n");

    return OK;
}

error_state Array_func(Array* heights, int64_t* max_area)
{
    Array stack;

    stack.size = 0;
    stack.data = malloc(0);
    *max_area = 0;
    Array_insert(&stack, -1, 0);

    for (size_t i = 0; i <= heights->size; ++i) {
        int height = i == heights->size ? 0 : heights->data[i];
        while ((stack.size > 0) && (height <= heights->data[stack.data[0]])) {
            int top = stack.data[0];
            Array_remove(&stack, 0);
            int width = stack.size == 0 ? i : i - stack.data[0] - 1;
            *max_area = MAX(*max_area, heights->data[top] * width);
        }
        Array_insert(&stack, i, 0);
    }

    return OK;
}
