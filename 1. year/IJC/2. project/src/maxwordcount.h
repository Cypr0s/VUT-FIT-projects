/** @headerfile  maxwordcount.h
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        20.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "htab.h"
#include "io.h"

#define MAX_WORD_LENGTH 256
#define HASH_TABLE_SIZE 16399 //'larger' prime number chosen for better distribution while prioritizing speed over memory

void find_max(htab_pair_t* data);

void print_max(htab_pair_t* data);