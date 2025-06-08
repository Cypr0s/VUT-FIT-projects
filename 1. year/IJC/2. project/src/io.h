/** @headerfile  io.h
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution b)
 *  @date        19.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <ctype.h>

int read_word(unsigned max, char s[max], FILE *f);

#endif