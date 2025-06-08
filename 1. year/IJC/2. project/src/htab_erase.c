/** @file        htab_erase.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

bool htab_erase(htab_t * t, htab_key_t key) {
    //get the hash, index of the key
    size_t idx = htab_hash_function(key) % t->arr_size;
    htab_item* item = t->items[idx];
    if(item == NULL) {
        return false;
    }

    htab_item* prev = NULL;
    htab_item* curr = item;
    while(curr != NULL){
        if(strcmp(curr->data.key, key) == 0) {
            //erasing the head of the linked list
            if(prev == NULL) {
                t->items[idx] = curr->next;
            }
            // erasing in the linked list
            else if(curr->next == NULL) {
                prev->next = NULL;
            }
            // erasing the tail of the linked list
            else{
                prev->next = curr->next;
            }
            free((char*)curr->data.key);
            free(curr);
            t->size--;
            return true;
        }
        //move forward
        prev = curr;
        curr = curr->next;
    }
    return false;
}