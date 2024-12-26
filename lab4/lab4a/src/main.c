#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "text/text.h"

int main(void)
{

    time_t begin = clock();
    Text tx;

    if (Text_create(&tx)) {
        Text_destruct(&tx);
        return EXIT_FAILURE;
    }

    Text_lengthify(&tx);

    FILE* fd = fopen("GOVNO.txt", "w+");
    Text_get_text(&tx, fd);
    fclose(fd);

    Text_destruct(&tx);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("time spent: %lf", time_spent);
    return EXIT_SUCCESS;
}
