/** Ethan Perry - Oct 30, 2024 - hasher_io
 * Implements a mining program that generates random values and counter, appends them, 
 * hashes them, and then checks for a specified number of leading zeros.
 *
 * This file includes several key functionalities:
 * - Parsing command-line arguments to get the input file, output file, and number of leading zeros.
 * - Reading the contents of the input file into a dynamically allocated array.
 * - Generating a random 6-digit number seeded by the current time of day.
 * - Updating a specified portion of the array with generated values.
 * - Converting hexadecimal characters to their decimal equivalents.
 * - Checking if a hashed value meets the required number of leading zeros.
 * - Iteratively mining for a valid hashed value that satisfies the leading zero requirement.
 */

#ifndef __HASHER_IO__
#define __HASHER_IO__

void parse_cmd(int, char**, char**, char**, int*);
char* read_file(char*, int*);
void output_to_file(char*, char*);

#endif