/** @file        htab_init.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

htab_t *htab_init(size_t n) {
    htab_t* t = malloc(sizeof(struct htab) + sizeof(htab_item*) * n);
    if (t == NULL)
        return NULL;
    t->arr_size = n;
    t->size = 0U;
    for(size_t i = 0; i < n; i++) {
        t->items[i] = NULL;
    }
    return t;
}