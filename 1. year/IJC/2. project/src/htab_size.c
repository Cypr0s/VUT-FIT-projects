/** @headerfile  htab_struct.h
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */

#include "htab_struct.h"

size_t htab_size(const htab_t * t) {
    return t->size;
}