/** @file        tail.c
 *  @author      Kristian Luptak
 *  @details     IJC-DU2, solution a)
 *  @date        18.4.2025
 *  @brief       Compiled: gcc 13.3.0
 */


#include "tail.h"

int main(int argc, char *argv[]) {
    unsigned int circ_buff_size = DEFAULT_PRINT_COUNT;
    FILE* file = NULL;
    
    // arg parser
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-n") == 0) {
            if(++i >= argc) {
                fprintf(stderr, "Missing size value after '-n' parameter try doing './tail -n size'\n");
                return 1;
            }
            char* err;
            int size = strtol(argv[i], &err, 10); // converting -n 'size'
            if(*err != '\0') {
                fprintf(stderr, " '%d' is Not a valid size for printing\n", size);
                return 1; 
            }
            if(size < 0) {
                fprintf(stderr, " '%d' is Not a valid size for printing\n", size);
                return 1;
            }
            if(size == 0) {
                return 0;
            }

            circ_buff_size = size;
        }
        else {
            file = fopen(argv[i], "r");
            if(file == NULL) {
                fprintf(stderr, "Error opening file \n");
                return 1;
            }
        }
    }
    if(file == NULL) {
        file = stdin;
    }

    circ_buff* cb = cbuf_create(circ_buff_size);
    char buff[MAX_LINE_LENGTH + 1]; // 4095 + \n + \0
    bool err_print = false;
    int i = 0;
    char c;
    // loading file into circ buffer
    while((c = getc(file)) != EOF) {
        // lines is shorter than MAX_LINE_LENGTH, load it into circ buffer
        if(c == '\n') {
            buff[i] = '\n';
            buff[i + 1] = '\0';
            cbuf_put(cb, buff);
            i = 0;
        }
        // if line contains more than MAX_LINE_LENGTH characters
        else if(i == MAX_LINE_LENGTH - 1) {
            if(!err_print) {
                fprintf(stderr,"error: found line longer than '%u' characters\n", MAX_LINE_LENGTH - 1);
                err_print = true;
            }
            // skipping all characters in the line
            while((c = getc(file)) != EOF && c !=  '\n') {
                continue;
            }

            buff[MAX_LINE_LENGTH - 1] = '\n'; //force setting ending ofr the string
            buff[MAX_LINE_LENGTH] ='\0';
            cbuf_put(cb, buff);
            i = 0;
        }
        else {
            buff[i++] = c;
        }
    }

    // line before EOF
    if(i > 0) {
        buff[i] = '\0';
        cbuf_put(cb, buff);
    }
    // printing last N lines
    for(unsigned i = 0; i < circ_buff_size; i++) {
        char* line = cbuf_get(cb);
        fprintf(stdout,"%s", line);
    }

    if (file != stdin) {
        fclose(file);
    }
    cbuf_free(cb);
    return 0;

}

/** @brief      Creates ring buffer wich holds char* of size n
 *  @param n    size of the to be ring buffer
 *  @returns    pointer to thecreated circ buff
 */
circ_buff* cbuf_create(int n) {
    // allocate struct
    circ_buff* cb = malloc(sizeof(circ_buff));
    if(cb == NULL) {
        exit(1);
    }
    // aloocate pointers to lines
    cb->lines = malloc(sizeof(char *) * n);
    if(cb->lines == NULL) {
        free(cb);
        exit(1);
    }
    // aloocate lines
    for(int i = 0; i < n; i++) {
        cb->lines[i] = malloc(sizeof(char) * (MAX_LINE_LENGTH + 1));
        if(cb->lines[i] == NULL) {
            free(cb->lines);
            free(cb);
            exit(1);
        }
    }
    // default values
    cb->size = n;
    cb->write = 0;
    cb->read = 0;
    return cb;
}

/** @brief      stores line into next index of circ buffer (rewrites old lines if theres a line already stored)
 *  @param cb   pointer to circ buff in which we will store the line
 *  @param line line which will be storesd
 *  @returns    nothing
 */
void cbuf_put(circ_buff* cb, char* line) {
    strcpy(cb->lines[cb->write], line);
    if(cb->write == cb->read){ 
        cb->read++;
        cb->read = cb->read % cb->size; // resets the read index if it matches size of the ring buffer
    }
    cb->write++;
    cb->write = cb->write % (cb->size); // resets the write index if it matches size of the ring buffer
}

/** @brief      finds and returns the line on the read index
 *  @param cb   pointer to circ buff from well get the line
 *  @returns    char *line
 */
char* cbuf_get(circ_buff* cb) {
    char* line = cb->lines[cb->read++];
    cb->read %= (cb->size); // reset the index
    return line;
    
}

/** @brief      frees allocated circ buff
 *  @param cb   pointer to circ buff which will be freed
 *  @returns    nothing
 */
void cbuf_free(circ_buff* cb) {
    for(unsigned i = 0; i < cb->size; i++) {
        free(cb->lines[i]);
    }

    free(cb->lines);
    free(cb);
}