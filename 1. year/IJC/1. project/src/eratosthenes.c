/* @file        eratosthenes.c
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a)
 * @date        13.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Eratosthenes sieve algorithm implementation
 */
#include "eratosthenes.h"

/* @brief   given a biset_t array of bits, sets all prime number bit indexes to 1 while all other bits are 0  
 * @param   array   array of X bits which are stored in unsigned longs */
void Eratosthenes(bitset_t array) {
    // filling the arraz with all ones
    bitset_fill(array, 1);
    
    // 0 and 1 are not prime numbers
    bitset_setbit(array, 0, 0);
    bitset_setbit(array, 1, 0);

    for(unsigned long i = 2; i <= sqrt(bitset_size(array)); i++) {
        if(bitset_getbit(array, i)) {
            // removing multiples of the number i
            for(unsigned long j = i + i; j < bitset_size(array); j += i) {
                bitset_setbit(array, j, 0);
            }
        }
    }
}