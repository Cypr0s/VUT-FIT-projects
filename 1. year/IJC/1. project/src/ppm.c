/* @file        ppm.c
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad b)
 * @date        17.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              Definition of ppm.h functions, reads ppm data from file and stores it into a 'ppm' struct
 */


#include "ppm.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* @brief   writes data from .ppm file into a dynamically allocated struct 'ppm', handles different errors
 * @param   filename     
 * @returns NULL        only if program encounters any error
 * @returns image       allocated struct with .ppm data */
struct ppm * ppm_read(const char *filename) {

    // opening file
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        warning("ppm_read: File couldnt be oppened");
        return NULL;
    }

    // loading the Format (should be "P6")
    char format[3];
    int scan_size = fscanf(file, "%2s", format);
    if(strcmp(format,"P6") != 0 || scan_size != 1) {
        warning("ppm_read: Invalid file format");
        fclose(file);
        return NULL;
    }

    // loading the Xsize, Ysize, "brightness" (should be 255)
    unsigned int xsize, ysize, max_color_brightness;

    scan_size = fscanf(file, "%u%u%u", &xsize, &ysize, &max_color_brightness);
    if(scan_size != 3 || xsize > LIMIT || ysize > LIMIT) {
        warning("ppm_read: xsize or ysize is bigger than %d", LIMIT);
        fclose(file);
        return NULL;
    }

    // checking "brightness"
    if(max_color_brightness != 255) {
        warning("ppm_read: Invalid brightness in ppm file value should be '255' ");
        fclose(file);
        return NULL;
    }

    // allocating structure
    struct ppm* image = malloc(sizeof(struct ppm) + (xsize * ysize * 3));
    if(image == NULL){
        fclose(file);
        error_exit("ppm_read: Failed to allocate memory for ppm structure");
    }
    
    fgetc(file); // there should be one whitespace after brightness

    for(size_t i = 0; i < xsize * ysize * 3; i++) {
        int value = fgetc(file);
        image->data[i] = value;
    }
    
    image->xsize = xsize;
    image->ysize = ysize;

    fclose(file);
    return image;
}

// @brief frees allocated struct
void ppm_free(struct ppm *p){
    free(p);
}