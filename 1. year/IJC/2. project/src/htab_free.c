/** @file        htab_free.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab_struct.h"

void htab_free(htab_t * t) {
    htab_clear(t);
    free(t);
}