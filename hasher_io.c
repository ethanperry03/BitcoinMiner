#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#define MEGA 1000000

/** void parse_cmd(int, char**, char**, char**, int*);
 * Parses command-line arguments and sets the input file, output file, and zeros value.
 * @param argv Number of command-line arguments.
 * @param argc Array of command-line argument strings.
 * @param infile Pointer to store the input file name.
 * @param outfile Pointer to store the output file name.
 * @param zeros Pointer to store the number of leading zeros.
 * @return void
 */
void parse_cmd(int argv, char** argc, char** infile, char** outfile, int* zeros){    
    // Ensure all options are entered into command line
    if (argv != 4) {
        printf("error: %d parameters received\nexpected three parameters\n", argv - 1);
        exit(-1);
    }

    // set infile to expected file name from cmd line input
    *infile = argc[1];

    // save outfile name
    *outfile = argc[2];

    // check that integer is reasonable
    *zeros = atoi(argc[3]);
    if (*zeros < 0 || *zeros > 256) {
        printf("%d is not a valid amount of preceding\nzeros for the hashed value\n", *zeros);
        exit(-1);
    }
}

/** char* read_file(char*, int*);
 * Reads a file into a dynamically allocated array and returns the offset.
 * @param filename Name of the file to read.
 * @param offset   Pointer to store the offset where the input ends.
 * @return char*   Pointer to the dynamically allocated array containing the file contents.
 */
char* read_file(char* filename, int* offset) {
    // dymaically allocate chunk of memory that can store 1MB of characters
    // 6 characters for rand, 6 characters for counter, and 1 for null term
    char* array = malloc(MEGA + 12 + 1);

    // open file to read
    FILE* infile = fopen(filename, "r");

    // check pointer is valid
    if (infile == NULL) {
        printf("Invalid input file, please try again\n");
        exit(-1);
    }

    // counter is 0
    int index = 0;
    // char
    char cha;
    // Read characters until the end of the file
    while ((cha = fgetc(infile)) != EOF) {
        // ignore newline and \r characters
        if (cha != '\n' && cha != '\r' && cha != '\t') {  
            array[index] = cha;
            index = index + 1;
        }
    }
    // close file pointer
    fclose(infile);
    // return the offset of where the input stuff ends 
    *offset = index;
    // null terminate
    array[index+12] = '\0';
    // return array
    return array;
}

/** void output_to_file(char* char*);
 * Writes the contents of a block to a specified output file.
 * @param block Pointer to the block of data to write.
 * @param outfileName Name of the output file.
 * @return void
 */
void output_to_file(char* block, char* outfileName) {
    // declare counter
    int counter = 0;
    
    // create file pointer and file to write output to
    FILE* outfile = fopen(outfileName, "w");

    // while you are not at a null char, read the next and write it
    while (block[counter] != '\0') {
        // write the char into the file
        fprintf(outfile, "%c", block[counter]);
        // update the counter
        counter = counter + 1;
    }

    // close off file pointer
    fclose(outfile);
}   