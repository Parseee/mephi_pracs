#include <assert.h>
#include <complex.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h> // replace with annealing
#include <sys/wait.h>

#include "error.h"

#define EPS 1e-9
#define INPUT_STRING_SIZE 100

ERROR_CODE compute(const double complex, const double, double complex* const);

int main(void)
{
    printf("Enter the equation's agrs\n");
    double complex x = 0;
    long long power = 0;
    char input[INPUT_STRING_SIZE];

    do {
        printf("x (in complex form (a+b*i)): ");
        fgets(input, INPUT_STRING_SIZE, stdin);
    } while (handle_input_complex(input, &x));

    // fprintf(stderr, "%f%+f*i\n", creal(x), cimag(x));

    do {
        printf("power (1+x)^power: ");
        fgets(input, INPUT_STRING_SIZE, stdin);
    } while (handle_input_int(input, &power));

    double complex result = CMPLX(0.0, 0.0);
    if (compute(x, power, &result)) {
        report_error(LOGIC_ERROR);
    }
    printf("result is %f%+f*i", creal(result), cimag(result));

    // double re, im;
    // scanf("%lg+%lg*i", &re, &im);
    // double complex z = CMPLX(re, im);

    // printf("%f%+f*i", creal(z), cimag(z));

    return EXIT_SUCCESS;
}

// todo assert nan isfinite zeroes nulls
ERROR_CODE compute(const double complex x, const double power, double complex* const res)
{
    *res = 1;
    double complex powr = CMPLX(creal(x), cimag(x));
    double complex add = CMPLX(0.0, 0.0);
    double fact = 1;
    double alpha = 1;
    double coef = power;
    int iters = 1;

    // TODO: break whenever shit happens
    do {
        // lassert(even == 0, "computation limit exeeded");
        // if () {
        //     break; // logic error? computation limit exeeded
        // }
        add = CMPLX((creal(powr)) * alpha / fact, cimag(powr));
        // if (fabs(add) < EPS) {
        //     break; // logic error? computation limit exeeded
        // }

        // fprintf(stderr, "aplha: %lf   factorial: %lf\n", alpha, fact);

        powr = CMPLX(creal(powr) * creal(x), cimag(powr) * cimag(x));
        iters += 1;
        fact *= iters;
        alpha *= coef;
        --coef;

        *res = CMPLX(creal(add) + creal(*res), cimag(add) + cimag(*res));
    } while (iters <= power);

    return CODE_OK;
}
