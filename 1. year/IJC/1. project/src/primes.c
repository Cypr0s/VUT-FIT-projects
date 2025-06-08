/* @file        primes.c
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a)
 * @date        13.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Finds all prime numbers from 0 to NUM_OF_BITS, prints NUM_OF_PRIMES largest prime number on stdout,
 *              also prints how much time it took to run the whole program 
 */


#include <time.h>       
#include <stdio.h>
#include "eratosthenes.h"
#include "bitset.h"


#define NUM_OF_BITS 333000000 // prime number sample size (will find primes in range "2" - NUMBER_OF_BITS)
#define NUM_OF_PRIMES 10  //how many prime numbers should be printed

int main() {
    clock_t start = clock();
    bitset_create(array, NUM_OF_BITS); // static arr
    Eratosthenes(array);

    unsigned long primes[NUM_OF_PRIMES];
    int count = 0;
    
    // stores primes into primes array, highest prime has the lowest index
    for(unsigned long i = bitset_size(array) - 1; i > 0 && count < NUM_OF_PRIMES; i--){
        if(bitset_getbit(array, i)){
            primes[count++] = i;
        }
    }
    // prints NUM_OF_PRIMES highest prime numbers(based on the size of array) from lowest to highest 
    for(int i = count; i > 0; i--) {
        fprintf(stdout, "%li\n", primes[i - 1]);
    }

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}