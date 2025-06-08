/*
    IZP Project 2
    Author: xluptak00
    Date:   Epoch 1732463920 seconds    */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MIN(a,b) (((a)<(b))?(a):(b))

// help message
const char* MESSAGE = 
    "IZP Project 2 - Bitmap Line and Square Finder, Author - xluptak00\n\n"
    "Commands:\n"
    "    --help          Display this help message.\n"
    "    test [filename] Test if the bitmap file is valid.\n"
    "    hline [filename] Find the longest horizontal line of 1's in the bitmap.\n"
    "    vline [filename] Find the longest vertical line of 1's in the bitmap.\n"
    "    square [filename] Find the largest square of 1's in the bitmap.\n\n"
    "Arguments:\n"
    "    [filename]  The name of the bitmap file to process. The file must contain a valid bitmap matrix.\n"
    "                First two values in the file represent the number of rows and columns in the matrix,\n"
    "                followed by the matrix itself, with '1' or '0' pixels.\n\n"
    "Examples:\n"
    "    program_name --help\n"
    "    program_name test bitmap.txt\n"
    "    program_name hline bitmap.txt\n";

//  Struct for saving the bitmap
typedef struct{
    unsigned long rows; //rows or columns in matrix cannot be lower than 0
    unsigned long columns;
    char* bitMatrix; // 2D matrix is represented as 1D array
} image;

//  Struct for saving the starting and ending point of object
typedef struct{
    unsigned long x; // (x, y) being the coords of point in matrix
    unsigned long y;
} point;

//  Declaration of functions
bool allocateImage(image *matrix, char *filename);

bool fillMatrix(image *matrix, FILE *file);

bool checkHline(image *matrix, point *start, point *end);

bool checkVline(image *matrix, point *start, point *end);

bool getBit(image *matrix, unsigned long countRow, unsigned long countCol);

bool checkSquare(image *matrix, point *start, point *end);

bool matrixCmd(char* command, char* filename);

unsigned long confirmPerimeter(image *matrix,unsigned long x,unsigned long y,unsigned long maxSize);

unsigned long longestLinePossible(image *matrix, unsigned long x, unsigned long y);

void cleanupImage(image *matrix);


int main(int argc,char *argv[]){
    if(argc == 1){
        fprintf(stdout, "%s", MESSAGE);
        return 1;
    }
    // if input argument is 'help' it prints out a help message about the function of the program
    else if(argc == 2){
        if(strcmp(argv[1], "--help") == 0){
            fprintf(stdout, "%s", MESSAGE);
            return 0;
        }
        return 1;
    }
    else if(argc == 3){
        if(matrixCmd(argv[1], argv[2])){
            return 1;
        }
        return 0;
    }
    else{
        fprintf(stdout, "%s", MESSAGE);
        return 1;
    }
    return 0;
}


/*  Helper function so the main doesnt get too junky
    Parameters: command(char[]), filename char[]
    Returns:    Bool 
                    True    = Program has encountetered and error
                    False   = program was succesful without encountering any Errors
                    ERROR = (invalid bitmap, wrong input parameters , ...)

    Function:   triggers different parts of code / algorithms based on the input arguments,
                prints found object coordinates   */
bool matrixCmd(char* command, char* filename){
    image matrix;
    point start; // for saving the coords of (lines/squares)
    point end;
    bool found = false; 
    if(strcmp(command, "test") == 0){
        if(!(allocateImage(&matrix, filename))){
            fprintf(stdout, "Valid\n");
            return 0;
        }
        fprintf(stderr, "Invalid\n");
        return 0;
    }
    if(allocateImage(&matrix, filename)){
        fprintf(stderr,"Invalid\n");
        return 0;
    }

    // Checks valid inputs being 'hline', 'vline' and 'square'
    if(strcmp(command, "hline") == 0){
        if(checkHline(&matrix, &start, &end)){
            found = true;
        }
    }
    else if(strcmp(command, "vline") == 0){
        if(checkVline(&matrix, &start, &end)){
            found = true;
        }
    }
    else if(strcmp(command, "square") == 0){
        if(checkSquare(&matrix, &start, &end)){
            found = true;
        }
    }
    else{
        //invalid input argument
        return 1;
    }

    cleanupImage(&matrix);

    // If the object was found
    if(found){
        fprintf(stdout,"%lu %lu %lu %lu \n", start.x, start.y, end.x, end.y);
    } else{
        fprintf(stdout, "Not found\n");
    }
    return 0;
}

/*  Parameters: struct image - in which the bitmap is stored, 
                filename -name of the file where the values which we read and work with are stored
    Returns:    Bool
                    True    = Program has encountetered and error
                    False   = program was succesful without encountering any Errors
                    ERROR = (invalid bitmap, invalid first 2 values for row, column , 
                            matrix size doesnt match the first two values(row,col) ...)
    Function:   Opens file to read it, allocates big enough space for whole matrix, 
                writes values into matrix   */
bool allocateImage(image *matrix,char *filename){
    FILE *file;
    file = fopen(filename, "r");

    if(file == NULL){
        return true;
    }

    //stores FIRST 2 values into image structure 
    if(fscanf(file, "%lu" "%lu", &matrix->rows, &matrix->columns) != 2){
        fclose(file);
        return true;
    }

    if(matrix->rows == 0 || matrix->columns == 0){
        fclose(file);
        return true;
    }

    //allocates char* of length which is determined by image rows, cols
    matrix->bitMatrix = (char *) malloc(sizeof(char) * matrix->rows * matrix->columns);
    if(matrix->bitMatrix == NULL){
        fclose(file);
        return true;
    }

    char c;
    unsigned long count = 0;
    bool foundSpace = false;
    // iterates every char in file
    while((c = fgetc(file)) != EOF){
        //skips spaces, tabs, '\n', ...
        if(isspace(c)){
            foundSpace = true;
            continue;
        }

        //In file there cannot be more characters that it is possible to store in matrix
        if(count >= matrix->rows * matrix->columns ||(c != '1' && c != '0')){
            free(matrix->bitMatrix);
            matrix->bitMatrix = NULL;
            fclose(file);
            return true;
        }
        /* stores the value in 'count'-th index of char* in image struct,
            increments 'count' after that    */
        if(foundSpace){
            matrix->bitMatrix[count++] = c;
            foundSpace = false;
        }
        else{
            free(matrix->bitMatrix);
            matrix->bitMatrix = NULL;
            fclose(file);
            return true;
        }
        }
    /*  checks if the count of the found bits in file matches the pre determined rows * cols
        (if the count is less there are surely missing values in the file)*/
    if(count != matrix->rows * matrix->columns) {
        free(matrix->bitMatrix);
        matrix->bitMatrix = NULL;
        fclose(file);
        return true;
    }
    fclose(file);
    return false;
}


/*  Parameters: Struct matrix {rows, cols, char[]}, 
                2x Struct point {X coord, Y coord} for starting and ending point
    Returns:    Bool     False when there are only zeroes in bitmap, otherwise true,
                Rewrites the values of start and point struct points
    Function:   Finds the longest HORIZONTAL line in matrix which is represented as 1D array
                and writes the starting and ending coords in points   */
bool checkHline(image *matrix, point *start, point *end){
    int maxLength = 0;
    //iterates every point by ROWS while increasing the line length by one
    for(unsigned long i = 0; i < matrix->rows; i++){
        int currLength = 0;
        for(unsigned long j = 0; j < matrix->columns; j++){
            // Checks for zero encounters
            if(!getBit(matrix, i, j)){
                // rewrites ending point, maxlength, resets current length
                if(currLength > maxLength){
                    maxLength = currLength;
                    end->x = i;
                    end->y = j - 1;
                }
                currLength = 0;
            }
            else{
                currLength++;
            }
        }
        // for lines at the end of row
        if(currLength > maxLength){
            maxLength = currLength;
            end->x = i;
            end->y = matrix->columns - 1;
        }
    }
    if(maxLength != 0){
        start->x = end->x;
        start->y = end->y - maxLength + 1;
        return true;
    }
    return false;
}    

/*  Parameters: Struct matrix {rows, cols, char[]}, 
                2x Struct point {X coord, Y coord} for starting and ending point
    Returns:    Bool     False when there are only zeroes in bitmap, otherwise true,
                Rewrites the values of start and point struct points
    Function:   Finds the longest VERTICAL line in matrix which is represented as 1D array
                and rewrites the starting and ending coords in point structs   */
bool checkVline(image *matrix, point *start, point *end){
    int maxLength = 0;

    //iterates every point in matrix by columns while increasing length of line
    for(unsigned long i = 0; i < matrix->columns; i++){
        int currLength = 0;
        for(unsigned long j = 0; j < matrix->rows; j++){

        //  Checks for zero 
            if(!getBit(matrix, j, i)){
                // rewrites ending point, maxlength, resets current length
                if (currLength > maxLength || (currLength == maxLength 
                    && (maxLength > 0 && j < end->x))) {
                    maxLength = currLength;
                    end->x = j - 1;
                    end->y = i;
                }
                currLength = 0;
            }
            else{
                currLength++;
            }
        }
        // for lines at the end of column
        if(currLength > maxLength){
            end->x = matrix->rows - 1;
            end->y = i;
            maxLength = currLength;
        }
    }
    if(maxLength != 0){
        start->x = end->x - maxLength + 1;
        start->y = end->y;
        return true;
    }
    return false;
} 

/*  HELPER FUNCTION for getting the value of 1D matrix
    Parameters: struct matrix{rows, cols, char[]}, Countrow - how many rows we will skip,
                countCol - how many columns we will skip
    Returns:    Bool based on the value in bitmap represented by 1D array, 
                which contains only '1's and '0's    1 = true, 0 = false
    Function:   returns true/ false based on the value of i-th index, i being calculated by formula
                (howManyRowsToSkip * lengthOfrow) + howManyColumnsToskip */
bool getBit(image *matrix, unsigned long countRow, unsigned long countCol){
    return matrix->bitMatrix[(countRow * matrix->columns)+ countCol] =='1' ? true : false;
}

/*  Parameters: Struct image{rows, columns, 1D matrix}, 
                2x Struct point{X coord, Y coord} for starting and ending point
    Returns:    Bool
                    True: at least one bit was true or found a square
                    False: whole bitmap is false
    Function:   Finds the largest boundary square in matrix which is represented as 1D array
                and rewrites the starting and ending coords in point structs*/
bool checkSquare(image *matrix, point *start, point *end){
    unsigned long maxSize = 0;    // tracks the largest square size found
    bool found = false;
    //iterates every bit in bitmap 
    for(unsigned long x = 0; x < matrix->rows; x++){
        for(unsigned long y = 0; y < matrix->columns; y++){
            if(getBit(matrix, x, y)){
                // finds largest valid square for bit
                unsigned long size = confirmPerimeter(matrix, x, y, maxSize);

                // for single bit 
                if (maxSize == 0 && !found) { 
                    found = true;
                    start->x = x;
                    start->y = y;
                    end->x = x;
                    end->y = y;
                }
                // if larger square is found rewrites coords
                if (size > maxSize) {
                    found = true;
                    maxSize = size;
                    start->x = x;
                    start->y = y;
                    end->x = start->x + maxSize;
                    end->y = start->y + maxSize;
                }
            }
        }
    }
    return found;
}


/*  HELPER function for finding the largest square
    Parameters: struct image{rows, cols, 1D bitmap array}, (X , Y)  - coords
    Returns:    unsigned long - longest possible uninterrupted line from starting point
    Function:   finds the longest possible uninterruped HORIZONTAL AND VERTICAL line from staring point(x,y)
                returns the size when a zero is encountered either in row or column */
unsigned long longestLinePossible(image *matrix, unsigned long x, unsigned long y){
    unsigned long sizeX = matrix->rows - x;
    unsigned long sizeY = matrix->columns - y;

    for(unsigned long size = 1; size < MIN(sizeX, sizeY);size++){
        if(!(getBit(matrix, x, y + size)) || (!getBit(matrix, x + size, y))){
            return size - 1; //largest size is the size before that
        }
    }
    // when no zeroes are encountered
    return MIN(sizeX, sizeY) - 1;
}


/*  HELPER function for finding the largest square
    Parameters: Struct image{rows cols char[]- 1D bitmap array}, (X, Y)- starting coords,
                maximum size of the square found yet
    Returns:    Largest possible square size from (x,y) coords, (x,y) being the top left corner 
    Function:   iterates every possible size from largest to smallest possible
                looking at bottom and right side of square bits,
                when a zero is encountered the size/ sqaure is invalid
                if no zeroes are encountered square is valid and size is returned*/
unsigned long confirmPerimeter(image *matrix, unsigned long x, 
                                unsigned long y, unsigned long maxSize){
    unsigned long size = longestLinePossible(matrix,x,y); // calculates largest possible size
    if(size <= maxSize) return maxSize; // when the largest possible square is smaller

    //itesrates every size
    for(;size > maxSize; size--){
        bool isValid = true;
        for(unsigned long i = size; i > 0;i--){
            if(!getBit(matrix, x + size, y + i)||(!getBit(matrix, x + i, y + size))){
                isValid = false;
                break;
            }
        }
        if (isValid) {
            return size;
        }
    }
    // if all squares are invalid
    return maxSize;
}

/*  Parameters: Struct image {rows, cols , char[]}
    Returns:    nothing
    FUnction:   frees allocated memory if its still allocated*/
void cleanupImage(image *matrix) {
    if (matrix->bitMatrix) {
        free(matrix->bitMatrix);
        matrix->bitMatrix = NULL;
    }
}
