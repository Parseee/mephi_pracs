#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #include "sort/sort.h"
#include "db/db.h"
#include "io_kit/io_kit.h"

int foo()
{
    // sry for this POSIX regex sucks ass
    const char* pattern = "^([[:alpha:]]*[[:space:]][[:alpha:]]*[[:space:]][[:alpha:]]*[[:space:]])([[:digit:]]{2}[[:upper:]]{2}-[[:upper:]]{4})([[:space:]][[:digit:]]*)$";

    // Compile the regex
    regex_t match;
    regmatch_t pmatch[4];
    int rc = regcomp(&match, pattern, REG_EXTENDED);
    if (rc) {
        fprintf(stderr, "Regex compilation failed\n");
        return 1;
    }

    char str[] = "razdvatri sosi hui 13ZV-ZXCV 2286661337";
    rc = regexec(&match, str, 4, pmatch, 0);

    if (rc == 0) {
        printf("Match found\n");
        // ssize_t len = pmatch[0].rm_eo - pmatch[0].rm_so;
        fprintf(stderr, "%s\n", str + pmatch[3].rm_so);
    } else if (rc == REG_NOMATCH) {
        printf("No match\n");
    } else {
        // regerror(rc, &match, NULL, 0);
        printf("Regex error\n");
    }

    return 0;
}

int main(void)
{
    // Text tx = (Text) { .data = NULL, .text = NULL, .data_size = 0 };
    // Text_create(&tx, "Onegin.txt");

    // fprintf(stderr, is_sorted(tx.text, tx.text_size, sizeof(*tx.text), Text_strcmp) ? "Sorted\n" : "Not sorted\n");

    // if (Text_sort(&tx)) {
    //     return EXIT_FAILURE;
    // }

    // for (size_t i = 0; i < tx.text_size; ++i) {
    //     fprintf(stderr, "%s\n", tx.text[i]);
    // }

    // Text_kill(&tx);

    Item* item = NULL;
    // construct_item(item, "acb abg iiif");

    foo();

    return EXIT_SUCCESS;
}