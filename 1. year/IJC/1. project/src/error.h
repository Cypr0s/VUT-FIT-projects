/* @headerfile  error.h
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad a), b)
 * @date        13.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Declaration of "print functions"
 */


#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

/* @brief   helper function for warning, error_exit function, prints fmt string into stderr
   @param   fmt     string which will be printed */
void warning(const char *fmt, ...);

/* @brief   prints "Warning: (fmt - message)" onto stderr
   @param   fmt     string which will be printed */
void error_exit(const char *fmt, ...);

/* @brief   prints "Warning: (fmt - message)" onto stderr, terminates program
   @param   fmt     string which will be printed */
void msg(const char *fmt, ...);

#endif
