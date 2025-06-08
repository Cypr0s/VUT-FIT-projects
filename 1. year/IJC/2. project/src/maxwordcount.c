/** @file        maxwordcount.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */

#include "maxwordcount.h"

unsigned max = 0;

int main() {
    char string[MAX_WORD_LENGTH];
    htab_t* t = htab_init(HASH_TABLE_SIZE); // create hashmap
    if(t == NULL)
    {
        fprintf(stderr, "maxwordcount:htab_init: Failed to allocate space for Hash Table\n");
        return 1;
    }
    // store words into hasmap
    while (read_word(MAX_WORD_LENGTH, string, stdin) != -1) {
        if(htab_lookup_add(t, string) == NULL) {
            fprintf(stderr, "maxwordcount:htab_lookup_add:Failed to find string %s\n", string);
            htab_free(t);
            return 1;
        }
    }
    htab_for_each(t, find_max);

    htab_for_each(t, print_max);
    htab_free(t);
    return 0;
}

/** @brief rewrites max variable if data has bigger occurence than max
 *  @param data consists of key which is stored in hash map, its occurence
 */
void find_max(htab_pair_t* data) {
    if(data->value > max) {
        max = data->value;
    }
}
/** @brief prints all keys from hash map which have "max" occurences (data)
 *  @param data consists of key which is stored in hash map, its occurence
 */
void print_max(htab_pair_t* data) {
    if(data->value == max) {
        printf("%s\t%u\n", data->key, data->value);
    }
}