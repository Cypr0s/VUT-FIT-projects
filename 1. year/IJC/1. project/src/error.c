/* @file        error.c
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a), b)
 * @date        13.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Definition of "print functions"
 */

#include "error.h"
#include <stdlib.h>
#include <stdio.h>

/* @brief   helper function for warning, error_exit function, prints fmt string
   @param   fmt     string which will be printed */
void msg(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
/* @brief   prints "Warning: (fmt - message)" onto stderr
   @param   fmt     string which will be printed */
void warning(const char *fmt, ... ) {
    fprintf(stderr,  "Warning: ");
    msg(fmt);
}

/* @brief   prints "Warning: (fmt - message)" onto stderr, terminates program
   @param   fmt     string which will be printed */
void error_exit(const char *fmt, ... ) {
    fprintf(stderr, "Error: ");
    msg(fmt);
    exit(EXIT_FAILURE);
}