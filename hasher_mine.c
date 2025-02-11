#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include "sha-256.h"
#define MEGA 1000000

/** int generate_rand();
 * Generates a random 6-digit number seeded by the current time of day.
 * @return int The generated random number.
 */
int generate_rand() {
    // get time of day
    struct timeval seed;
    gettimeofday(&seed, NULL);
    // seed random with the time of day
    srand(seed.tv_sec*MEGA + seed.tv_usec);
    // get random 6 digit number
    int num = rand() % MEGA;
    // return
    return num;
}

/** char* update_value(char*, int, int);
 * Updates the given array at the specified offset with a given value, 
 * formatted as a 6-digit number (either the random num or the counter).
 * @param array The array to be updated.
 * @param offset The starting index to update in the array.
 * @param val The value to be inserted into the array.
 * @return char* The updated array.
 */
char* update_value(char* array, int offset, int val) {
    // index starts at the back of the number (+5 to the starting index)
    int index = 5;
    // while the number is still being processed
    while(index >= 0) {
        // at position offset+index, get the digit by %10 and cast as char
        array[offset+index] = '0' + (val % 10);
        // decrement index
        index = index - 1;
        // if there are still digits to process        
        if (val > 0){
            // int divide the least sig digit off the number
            val = val / 10;
        }
    }
    // return the array that is updated
    return array;
}

/** int hex_to_decimal(char);
 * Converts a hexadecimal character to its decimal value.
 * @param hex  The hexadecimal character.
 * @return int The corresponding decimal value.
 */
int hex_to_decimal(char hex) {
    // For characters '0' to '9'
    if (hex >= '0' && hex <= '9') {
        return (hex - '0'); 
    } 
    // For characters 'a' to 'f'
    else if (hex >= 'a' && hex <= 'f') {
        return (hex - 'a' + 10); 
    }
    // return error code
    else {
        printf("Invalid hex value\n");
        exit(-1);
    }
}

/** int check_hashed(char*, int);
 * Checks if the hashed value has the required number of leading zeros.
 * @param hashed The hashed value in hexadecimal format.
 * @param zeros  The number of leading zeros required.
 * @return int   Return 1 if the hashed value meets the requirement, otherwise 0.
 */
int check_hashed(char* hashed, int zeros) {
    // find number of leading 0's in hex (chunks of 4's)
    int zero_head = zeros / 4;
    // find the number of zeros in the first byte after all zero's
    int zero_mod = zeros % 4;

    // skip through necessary leading zeros 
    for (int i = 0; i < zero_head; i++) {
        // if it is not a leading zero, then this nonce does not work
        if (hashed[i] != '0') {
            // return 0 to stop checking this nonce
            return 0;
        }
    }

    // turn the first non zero leading hex digit into decimal
    int digit = hex_to_decimal(hashed[zero_head]);

    // cases for how many leading 0's there were, return 1 if success
    if ((zero_mod == 0) || 
        (zero_mod == 1 && digit < 8) || 
        (zero_mod == 2 && digit < 4) || 
        (zero_mod == 3 && digit <= 1)) {
        return 1;
    } 
    // not enough leading zero's, return 0, and try again
    else {
        return 0;
    }
}   

/** char* mining(char*, int, int);
 * Mines for a valid hashed value with the specified number of leading zeros.
 * @param array  The array containing the data to be hashed.
 * @param offset The offset in the array where the nonce starts.
 * @param zeros  The number of leading zeros required in the hashed value.
 * @return char* The array with the valid hashed value for output.
 */
char* mining(char* array, int offset, int zeros) {
    // initialize counter variable and val to store randnum seed
    int counter, randnum, hash_flag;
    // alloc memeory to store hashed value
    char* hashed = malloc(65 * sizeof(char));

    // assume you have not a valid hashed value
    hash_flag = 0;
    while (hash_flag == 0) {
        counter = 0;
        // generate random, 6-digit number
        randnum = generate_rand();
        // update value in the block by putting random number in and starting the counter
        array = update_value(array, offset, randnum);

        // while the counter is not max and flag not found
        while (counter < MEGA && !hash_flag) {
            // update counter value in the nonce
            array = update_value(array, offset+6, counter);

            // hash and check value
            sha_256_string(hashed, array, offset+12);

            // check if the hashed value satisfies number of leading zeros
            hash_flag = check_hashed(hashed, zeros);

            // update counter
            counter = counter + 1;
        }
    }
    // free memory
    free(hashed);
    // return the array
    return array;
}