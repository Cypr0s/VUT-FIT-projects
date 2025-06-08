/* @headerfile  bitset.h 
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a)
 * @date        10.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Definition of functions and macros for Bitset_t array manipulation 
 */


#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "error.h"


typedef unsigned long* bitset_t;
typedef unsigned long bitset_index_t;

// @brief   calculates index of bit in unsigned long array(for example 65 gives index 1)
#define BIT_INDEX(index) ((index) / (sizeof(unsigned long) * CHAR_BIT))

// @brief   calculates calculates the position of bit in unsigned long array based on index (73 gives gives 9)
#define BIT_POSITION(index) ((index) % (sizeof(unsigned long) * CHAR_BIT))

/* @brief   calculates how many unsigned longs are needed for N bits based on bit_position,
 *          if theres remainder we need one more unsigned long */
#define bitset_size_calc(n) (BIT_POSITION(n) == 0 ? (BIT_INDEX(n)) : (BIT_INDEX(n) + 1))


/* @brief   creates a static unsigned long array (size is based on size paramater), checks for bounds of unsigned long,
 *          stores the size in first unsigned long, fills the array with 0
 * @param   array_name  the name of the array to be created
 * @param   size        size of the array which will be created (its defined by bits so it needs to be converted)
*/
#define bitset_create(array_name, size) \
    static_assert(size > 0); \
    static_assert(ULONG_MAX > size); \
    unsigned long array_name[bitset_size_calc(size) + 1] = {size, 0 };


/* @brief   creates a dynamically allocated unsigned long array (size is based on size paramater), 
 *          checks for bounds of unsigned long, stores the size in first unsigned long, fills the array with 0
 * @param   array_name  the name of the array to be allocated
 * @param   size        size of the array which will be allocated 
 *                      (its defined by bits so it needs to be converted, check bitset_size_calc)
*/
#define bitset_alloc(array_name, size) \
    assert(size > 0); \
    assert(bitset_size_calc(size) <= ULONG_MAX); \
    bitset_t array_name = (bitset_t) calloc((bitset_size_calc(size) + 1), sizeof(unsigned long)); \
    if(array_name == NULL) { \
        error_exit("bitset_alloc: Chyba alokace paměti"); \
    } \
    array_name[0] = size;

// INLINE functions definitions
#ifdef USE_INLINE

    // @brief   frees allocated array_name
    static inline void bitset_free(bitset_t array_name) {
        free(array_name);
    }

    /* @brief   returns size of array_name in bits
     * @return  (unsigned long) size of array in bits*/
    static inline unsigned long bitset_size(bitset_t array_name) {
        return array_name[0];
    }

    /* @brief   returns the bit on index-th position, checks for size bounds(check bitset_size)
     * @param   array_name  name of the array from which well get the bit
     * @param   index       index of the bit we want to return 
     * @return  (unsigned long)bit on the index-th position  */
    static inline unsigned long bitset_getbit(bitset_t array_name, bitset_index_t index) {
        if(index > bitset_size(array_name)){
            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array_name));
        }
        return ((array_name[1 + BIT_INDEX(index)] >> (BIT_POSITION(index))) & 1);
    }

    /* @brief   fills the array with 0 or 1 bits based on the bool_variable
     * @param   array_name      name of the array which we want to fill
     * @param   bool_variable   decides if the array fills with ones or zeroes */
    static inline void bitset_fill(bitset_t array_name, int bool_variable) {
        unsigned long size = bitset_size_calc(bitset_size(array_name));
        for(unsigned long i = 0; i < size; i++) {
            if(bool_variable) {
                array_name[1 + i] |= ULONG_MAX;
            }
            else {
                array_name[1 + i] &= 0;
            }
        }
    }

    /* @brief   sets the index-th bit of array_name to 0 or 1 based on bool_variable 
     * @param   array_name      name of the array which we'll manipulate
     * @param   index           position of the bit which we'll set
     * @param   bool_variable   decides what will the bit set value to */
    static inline void bitset_setbit(bitset_t array_name,  bitset_index_t index, int bool_variable) {
        if(index >= bitset_size(array_name)) {
            error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array_name));
        }
        if(bool_variable) {
            array_name[1 + BIT_INDEX(index)] |= (1UL << BIT_POSITION(index));
        } else {
            array_name[1 + BIT_INDEX(index)] &= ~(1UL << BIT_POSITION(index));
        }
    }

// MACROS definition 
#else

    // @brief   frees allocated array_name
    #define bitset_free(array_name) (free(array_name))

    // @brief   returns (unsigned long) size of the array which is on the first index
    #define bitset_size(array_name) (array_name[0])

    // @brief   fills the array with 0 or 1 bits based on the bool_variable
    #define bitset_fill(array_name, bool_variable) do { \
        unsigned long size = bitset_size_calc(bitset_size(array_name)); \
        for (unsigned long i = 0; i < size; i++) { \
            if(bool_variable) { \
                array_name[1 + i] |= ULONG_MAX; \
            } \
            else { \
                array_name[1 + i] &= 0; \
            } \
        } \
    }while (0)

    // @brief   returns the bit on index-th position, checks for size bounds(check bitset_size)
    #define bitset_getbit(array_name, index) \
        ((index >= bitset_size(array_name)) ? \
        (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array_name)), 0) : \
        ((array_name[1 + BIT_INDEX(index)] >> (BIT_POSITION(index))) & 1))

    // @brief   sets the index-th bit of array_name to 0 or 1 based on bool_variable 
    #define bitset_setbit(array_name, index, bool_variable) do { \
        if ((index) >= (bitset_size(array_name))) { \
            error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array_name)); \
        } \
        if (bool_variable) {\
            array_name[1 + BIT_INDEX(index)] |= (1UL << BIT_POSITION(index)); \
        } else { \
            array_name[1 + BIT_INDEX(index)] &= ~(1UL << BIT_POSITION(index)); \
        } \
    } while (0)


#endif
#endif