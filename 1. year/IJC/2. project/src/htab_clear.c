/** @file        htab_clear.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */

#include "htab_struct.h"

// clears all items in hashmap
void htab_clear(htab_t * t) {
    t->size = 0;
    for(size_t i = 0; i < t->arr_size; i++) {
        htab_item *item = t->items[i];
        htab_item *prev = NULL;
        while(item != NULL) {
            prev = item;
            item = item->next;
            free((char*)prev->data.key);
            free(prev);
        }
        t->items[i] = NULL;
    }
}