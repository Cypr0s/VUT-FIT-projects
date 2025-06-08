/** @file        htab_for_each.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

void htab_for_each(const htab_t * t, void (*f)(htab_pair_t* data)) {
    for(size_t i = 0; i < t->arr_size; i++) {
        htab_item *item = t->items[i];
        while(item != NULL) {
            f(&item->data);
            item = item->next;
        }
    }
}