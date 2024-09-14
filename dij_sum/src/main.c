#include <stdio.h>
#include <limits.h>


enum ERROR_CODE { OK = 0, INTEGER_OVERFLOW };

enum ERROR_CODE dij_sum(long long *, long long);
int HANDLE_ERROR(const enum ERROR_CODE);


int main(const signed argc, const char* argv[]) {
    long long x;
    scanf("%lli", &x);
    long long sum = 0;
    if (HANDLE_ERROR(dij_sum(&sum, x))) {
        return 1;
    }

    printf("%lli", sum);
    return 0;
}

enum ERROR_CODE dij_sum(long long *sum, long long x) {
    while(x > 0) {
        *sum += x % 10;
        x /= 10;
    }
    return OK;
}

int HANDLE_ERROR(const enum ERROR_CODE e) {
    switch (e) {
        case OK:
            fprintf(stderr, "OK\n");
            return 0;
            break;
        case INTEGER_OVERFLOW:
            fprintf(stderr, "INTEGER OVERFLOW\n");
            return 1;
            break;
        default:
            fprintf(stderr, "SOMETHING HAPPENED\n");
            return 1;
            break;
    }
}
