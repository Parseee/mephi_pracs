#include "db.h"
#include "../util.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#define DEFAULT_CAPACITY 1000

DB_state DB_create(DB* db)
{
    assert(db);
    db->capacity = DEFAULT_CAPACITY;
    db->size = 0;
    db->data = calloc(db->capacity, sizeof(db->data));

    return DB_OK;
}

DB_state DB_destruct(DB* db)
{
    assert(db);
    if (!db->data) {
        report_error("database is not init", DB_INTERNAL_ERROR);
    }
    for (size_t i = 0; i < db->capacity; ++i) {
        if (db->data[i]) {
            free(db->data[i]->name);
            free(db->data[i]->id);
        }
    }
    free(db->data);
    db->data = NULL;
    db->size = 0;
    db->capacity = 0;

    return DB_OK;
}

DB_state DB_append(DB* db, Item* item)
{
    assert(db);
    if (db->size + 1 > db->capacity) {
        Item** new_data = NULL;
        if ((new_data = realloc(db->data, db->capacity * 2 * sizeof(db->data))) == NULL) {
            free(db->data);
            db->size = 0;
            db->capacity = 0;
            report_error("can't realloc", DB_INTERNAL_ERROR);
        }
        db->capacity *= 2;
        db->data = new_data;
    }

    db->data[db->size] = item;
    item = NULL;
    db->size += 1;
    return DB_OK;
}

// bool DB_strcmp(const void* l_t, const void* r_t) {
//     assert(l_t && "");
//     assert(r_t && "");

//     const char* l = *(char**)l_t;
//     const char* r = *(char**)r_t;

//     size_t len_l = 0;
//     char* end_l = NULL;
//     // char* new_l = ignore_n(l, end_l, &len_l);

//     size_t len_r = 0;
//     char* end_r = NULL;
//     // char* new_r = ignore_n(r, end_r, &len_r);

//     bool flag = false;
//     if (len_l == len_r) {
//         for (int i = 0; i < len_l; ++i) {
//             if (tolower(new_l[i]) > tolower(new_r[i])) {
//                 flag = false;
//                 break;
//             }
//         }
//         flag = true;
//     } else if (len_l < len_r) {
//         flag = true;
//     } else {
//         flag = false;
//     }

//     free(new_l);
//     free(new_r);
//     return flag;
// }
