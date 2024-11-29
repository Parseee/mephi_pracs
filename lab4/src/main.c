#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger/logger.h"
#include "sort/sort.h"
#include "text/text.h"

// #define DEBUG

int main(void)
{
    log_create();

    Text tx = (Text) { .data = NULL, .text = NULL, .data_size = 0 };
    Text_create(&tx, "Onegin.txt");

    fprintf(stderr, is_sorted(tx.text, tx.text_size, sizeof(*tx.text), Text_strcmp) ? "Sorted\n" : "Not sorted\n");

    if (Text_sort(&tx)) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tx.text_size; ++i) {
        fprintf(stderr, "%s\n", tx.text[i]);
    }

    Text_kill(&tx);

    // char* a = "123123";
    // char* b = "abcabc";

    // swap(&a, &b, sizeof(a));

    // fprintf(stderr, "%s, %s\n", a, b);

    return EXIT_SUCCESS;
}

/*
- [x] Первая версия
    - [x] Ввод с файла при помощи fgets
    - [x] Прямоугольный массив строк (с константными размерами)
    - [x] Свой strcmp (который скипает пунктуацию и пробельные символы)
    - [x] Тестирование (is_sorted())
    - [x] Сортировка пузырьком на void*
- [x] Вывод в файл
*/