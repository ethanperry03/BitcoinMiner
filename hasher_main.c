/** Hasher main - Ethan Perry - Oct. 18th 
* This is the main file that uses the hasher functions and helpers to simulate
* hashing a block to a sufficient amount of zero's. The main function takes cmd
* line arguments, specifying name of input text file, output file, and number of
* leading zeros to be considered a successful hashed block. Main parses the cmd 
* line arguments and does some error checking before continuing. Then, the input 
* file is read and the text for the block is stored in main. Mining() is called 
* and loops until a nonce satifies the conditions. Whatever block led to that 
* succesful hash is then outputted to the outfile with the appended random 
* number and counter at success.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>  
#include "hasher_io.h"  
#include "hasher_mine.h"

int main(int argv, char** argc) {
    // declare variables
    int zeros, offset;
    char* outfile;
    char* infile;

    // parse the cmd line input and return by "reference" to store vars in main
    parse_cmd(argv, argc, &infile, &outfile, &zeros);

    // store the block text by reading in the file and recording the amount in the array
    char* block = read_file(infile, &offset);

    // do the mining loop to try nonces and find sufficient hashed value
    block = mining(block, offset, zeros);

    // output to file and pass output file name to create file
    output_to_file(block, outfile);

    // free memory
    free(block);

    return 0;
}