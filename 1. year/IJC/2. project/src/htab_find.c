/** @file        htab_find.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

htab_pair_t* htab_find(const htab_t * t, htab_key_t key) {
    if(t->size == 0){
        return NULL;
    }

    size_t idx = htab_hash_function(key) % t->arr_size;
    htab_item* item = t->items[idx];

    while(item != NULL) {
        htab_pair_t item_data = item->data;
        if(strcmp(item_data.key, key) == 0) {
            return &item->data;
        }
        item = item->next;
    }
    return NULL;
}