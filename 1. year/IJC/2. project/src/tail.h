/** @headerfile  tail.h
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution a)
 *  @date        18.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */

#ifndef TAIL_H
#define TAIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 4096
#define DEFAULT_PRINT_COUNT 10

// circ buff struct
typedef struct {
    char **lines;
    unsigned int size;
    int write;
    int read;
} circ_buff;


void cbuf_free(circ_buff* cb);

circ_buff* cbuf_create(int n);

char* cbuf_get(circ_buff* cb);

void cbuf_put(circ_buff* cb, char* line);

#endif //TAIL_H