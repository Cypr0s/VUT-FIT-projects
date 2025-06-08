/** @headerfile  htab_struct.h
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#ifndef HTAB_STRUCT
#define HTAB_STRUCT

#include "htab.h"
#include <stdlib.h>

struct item {
    htab_pair_t data;
    struct item* next;
};

typedef struct item htab_item;

struct htab {
    unsigned arr_size;
    unsigned size;
    htab_item* items[];
};

#endif //HTAB_STRUCT