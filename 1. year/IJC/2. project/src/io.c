/** @file        io.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */



#include "io.h"
/** @brief stores one word of max size 'max' into string 's' from file 'f'
 *  @param max how many characters we will store
 *  @param s string in which we will store the characters
 *  @param f pointer to the file
 *  @returns int - length of the word or EOF
 */
int read_word(unsigned max, char s[max], FILE *f) {
    char c =getc(f);

    // skip leading whitespaces
    while (c != EOF && isspace(c)) {
        c = getc(f);
    }

    if (c == EOF) {
        return -1;
    }

    // store the word
    unsigned i = 0;
    while(c != EOF && !isspace(c) && i < max - 1) {
        s[i++] = c;
        c = getc(f);
    }
    s[i] = '\0';
    return i;
}