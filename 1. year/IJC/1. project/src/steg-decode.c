/* @file        steg-decode.c
 * @author      Kristian Luptak
 * @details     IJC-DU1, priklad b)
 * @date        21.3.2025
 * @brief       Přeloženo: gcc 13.3.0
 *              finds "hidden" message in ppm file which is stored in bits on prime number bytes, 
 *              prints it on stdout
 */


#include "ppm.h"
#include "eratosthenes.h"
#include <stdio.h>
#include <stdlib.h>


#define START_PRIME 101


/*          taken from https://www.cl.cam.ac.uk/~mgk25/ucs/utf8_check.c
 * @brief   function checks if the input string is in UTF-8
 * @return  Null if the string is valid
*/
unsigned char *utf8_check(unsigned char *s)
{
  while (*s) {
    if (*s < 0x80)
      /* 0xxxxxxx */
      s++;
    else if ((s[0] & 0xe0) == 0xc0) {
      /* 110XXXXx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[0] & 0xfe) == 0xc0)                        /* overlong? */
	return s;
      else
	s += 2;
    } else if ((s[0] & 0xf0) == 0xe0) {
      /* 1110XXXX 10Xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[0] == 0xe0 && (s[1] & 0xe0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xed && (s[1] & 0xe0) == 0xa0) ||    /* surrogate? */
	  (s[0] == 0xef && s[1] == 0xbf &&
	   (s[2] & 0xfe) == 0xbe))                      /* U+FFFE or U+FFFF? */
	return s;
      else
	s += 3;
    } else if ((s[0] & 0xf8) == 0xf0) {
      /* 11110XXX 10XXxxxx 10xxxxxx 10xxxxxx */
      if ((s[1] & 0xc0) != 0x80 ||
	  (s[2] & 0xc0) != 0x80 ||
	  (s[3] & 0xc0) != 0x80 ||
	  (s[0] == 0xf0 && (s[1] & 0xf0) == 0x80) ||    /* overlong? */
	  (s[0] == 0xf4 && s[1] > 0x8f) || s[0] > 0xf4) /* > U+10FFFF? */
	return s;
      else
	s += 4;
    } else
      return s;
  }

  return NULL;
}


int main(int argc, char* argv[]) {
    if(!(argc == 2)) {
        error_exit("steg-decode: Wrong number of arguments");
    }

    struct ppm* image = ppm_read(argv[1]);
    if(!image) {
      error_exit("steg-decode: Failed to create .ppm data");
    }

    bitset_alloc(arr,  image->xsize * image->ysize * 3);
    Eratosthenes(arr);

    // dynamically allocated string which enlarges with every character
    char* message = NULL;
    int message_size = 0;

    message = malloc(sizeof(char) * (message_size + 1)); // message size + 1 for \0
    if (message == NULL) {
      bitset_free(arr);
      ppm_free(image);
      error_exit("steg-decode: Failed to allocate memory for message");
    }

    // storing the bits of the "to be" character
    unsigned int bit_pos = 0;
    unsigned int character_arr[CHAR_BIT];

    // looping all numbers from START_PRIME to size of ppm and choosing only prime ones
    for(bitset_index_t i = START_PRIME; i <  image->xsize * image->ysize * 3; i++) {
        if(bitset_getbit(arr, i)) {

            // getting the LSB form byte in image
            character_arr[bit_pos++] = image->data[i] & 1;
            if(bit_pos == CHAR_BIT) {

                // 'creating' the character
                char character = 0;
                for(size_t j = 0; j < CHAR_BIT; j++) {
                    character |= character_arr[j] << j;
                }
                bit_pos = 0;

                message[message_size++] = character;

                if (character == '\0') { 
                  break;
                }

                // reallocing string for new possible character
                message = realloc(message, message_size + 1); // message size + 1 for \0
                if (message == NULL) {
                  free(message);
                  bitset_free(arr);
                  ppm_free(image);
                  error_exit("steg-decode: Failed to reallocate memory for message");
                }
            }
        }
    }

    if(utf8_check((unsigned char*)message) != NULL) {
      free(message);
      bitset_free(arr);
      ppm_free(image);
      error_exit("steg-decode: String is not in valid UTF-8 format");
    }

    printf("%s\n", message);
    free(message);
    bitset_free(arr);
    ppm_free(image);

    return 0;
}