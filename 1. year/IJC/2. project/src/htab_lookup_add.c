/** @file        htab_lookup_add.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

htab_pair_t* htab_lookup_add(htab_t* t, htab_key_t key) {
    // checking for the key
    htab_pair_t* pair = htab_find(t, key);
    if(pair != NULL) {
        pair->value++;
        return pair;
    }
    // finding the hash, index where should we store the key
    size_t idx = htab_hash_function(key) % t->arr_size;
    htab_item* item = t->items[idx];

    htab_item* new_item = malloc(sizeof(htab_item));
    if(new_item == NULL){
        return NULL;
    }
    //creating the key
    new_item->next = NULL;
    new_item->data.value = 1;
    new_item->data.key = malloc(sizeof(char) * (strlen(key) + 1));
    if(new_item->data.key == NULL){
        free(new_item);
        return NULL;
    }
    strcpy((char*)new_item->data.key, key);

    // gotta love leetcode single linked list problems
    //ading the key to inex-th linked list
    if(item != NULL) {
        while(item->next != NULL) {
            item = item->next;
        }
        item->next = new_item;
    }
    else {
        t->items[idx] = new_item;
    }
    t->size++;
    return &new_item->data;
}