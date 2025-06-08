/* @headerfile  ppm.h
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad b)
 * @date        17.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Declaration of ppm functions, struct, defined size limit 
 */


#ifndef PPM_H
#define PPM_H


#define LIMIT 16000 // limit for xsize and ysize

// struct for storing the .ppm data
struct ppm {
    unsigned xsize;
    unsigned ysize;
    char data[];    // RGB bajty, celkem 3*xsize*ysize
};

/* @brief   writes data from .ppm file into a dynamically allocated struct 'ppm', handles different errors
 * @param   filename     
 * @returns NULL        only if program encounters any error
 * @returns image       allocated struct with .ppm data */
struct ppm * ppm_read(const char * filename);


// @brief   frees allocated ppm
void ppm_free(struct ppm *p);

#endif