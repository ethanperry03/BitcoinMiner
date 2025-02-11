# Hash Mining Simulation

## Overview
This project implements a hash mining simulation in C, utilizing random number generation, multiple processes, and performance measurement. The project consists of two programs: `hasher` for mining a valid nonce and `main` for managing multiple mining processes.

## Features
- Random nonce generation for hash computation.
- Multi-process execution to improve efficiency.
- Uses SHA-256 hashing.
- Measures execution time of the mining process.
- Debugging with GDB and Valgrind.

## Compilation & Execution
### Build
```sh
make
```

### Run
#### hasher
```sh
./hasher <input_file> <output_file> <num_zeroes>
```
#### main
```sh
./main <input_file> <num_zeroes> <num_processes>
```

### Examples
```sh
./hasher data.txt result.txt 5
./main data.txt 5 4
```

## Implementation Details
The project is organized into two main programs:
- `hasher.c`: Reads an input file, appends a nonce, and computes a SHA-256 hash until it meets the required number of leading zeroes in binary.
- `main.c`: Spawns multiple `hasher` processes, waits for one to complete, then terminates the others and prints the successful output file.

### Hash Computation
- Reads file contents into a buffer (up to 1,000,000 characters).
- Appends a random 6-digit number and iterates through possible nonces.
- Converts the SHA-256 hash from hex to binary to check for leading zeroes.
- Uses `srand(gettimeofday)` for precise randomization.

### Multi-Process Execution
- `main.c` starts multiple `hasher` processes with different output file names (`out0.txt`, `out1.txt`, etc.).
- Waits for one process to succeed, then terminates the others using signals.
- Ensures no zombie processes by reaping terminated children.

### Timing Execution
Uses `gettimeofday` to measure execution time accurately.

## Debugging
Use GDB and Valgrind to debug memory errors:
```sh
gdb ./hasher
valgrind --leak-check=full ./hasher data.txt result.txt 5
```

## Code Style & Best Practices
- No global variables; use function parameters.
- Functions should ideally be ≤ 20 lines.
- Use `malloc()` for dynamic memory allocation.
- Handle file errors gracefully.

## Additional Notes
- `CTRL+C` terminates an infinite loop.
- Ensure proper cleanup of child processes in `main.c`.
