#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h> // replace with annealing

#include "error.h"

#define EPS 1e-9
#define INPUT_STRING_SIZE 100

ERROR_CODE handle_input_int(const char*, int64_t*);
ERROR_CODE handle_input_floating(const char*, float*);
ERROR_CODE compute(float const, int64_t const, float* const);

int main(void) {
    printf("Enter the equation's agrs\n");
    float x = 0;
    long long accuracy = 0;
    char input[INPUT_STRING_SIZE];

    printf("x: ");
    do {
        fgets(input, INPUT_STRING_SIZE, stdin);
        
    } while (handle_error(handle_input_floating(input, &x)));

    printf("accuracy (10^-n): ");
    do {
        fgets(input, INPUT_STRING_SIZE, stdin);
    } while (handle_error(handle_input_int(input, &accuracy)));

    float result = 0.0;
    compute(x, accuracy, &result);
    printf("result is %.9f and %.9f", result, log(x + sqrt(1 + x * x)));

    return EXIT_SUCCESS;
}

// todo assert nan isfinite zeroes nulls
ERROR_CODE compute(float const x, int64_t const accuracy, float* const res) {
    float sign = -1;
    float odd = 1;
    float even = 2;
    float iters = 1;

    *res = x;
    float powr = x * x * x;
    float add = 0.0;
    float reference = pow(1, -1 * accuracy);
    do {
        if (even == 0) {
            break; // logic error? computation limit exeeded
        }
        add = sign * (odd / even) * (powr / (2 * iters + 1));
        if (fabs(add) < EPS) {
            break; // logic error? computation limit exeeded
        }
        sign *= -1;
        odd *= (2 * iters) + 1;
        even *= 2 * (iters + 1);
        powr *= x * x;
        iters += 1;

        *res += add;
    } while (fabs(add) <= reference);

    return CODE_OK;
}
