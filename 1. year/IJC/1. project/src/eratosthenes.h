/* @headerfile  eratosthenes.h
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a)
 * @date        13.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Eratosthenes sieve algorithm declaration, includes
 */

 #ifndef ERATOSTHENES_H
 #define ERATOSTHENES_H
 
 #include "bitset.h"
 #include <math.h>
 
 /* @brief   given a biset_t array of bits, sets all prime number bit indexes to 1 while all other bits are 0  
  * @param   array   array of X bits which are stored in unsigned longs */
 void Eratosthenes(bitset_t array);
 
 #endif