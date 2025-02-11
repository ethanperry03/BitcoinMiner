/** Ethan Perry - Oct 30, 2024 - hasher_main
 * Contains functions for generating random numbers, updating arrays, converting hexadecimal 
 * characters, checking hashed values, and mining for valid hashes.
 *
 * This file includes the following functionalities:
 * - Generating random 6-digit numbers using the current time as a seed.
 * - Updating a specified portion of an array with generated values or counters.
 * - Converting hexadecimal characters to their decimal values.
 * - Checking if a hashed value meets the required number of leading zeros.
 * - Iteratively mining for a valid hashed value that satisfies the leading zero requirement.
 */

#ifndef __HASHER_MINE__
#define __HASHER_MINE__

int generate_rand();
int check_hashed(char*, int);
int hex_to_decimal(char);
char* update_value(char*, int, int);
char* mining(char*, int, int);

#endif