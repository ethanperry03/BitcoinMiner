/** Main of WheatCoin Program - Oct 30 - Ethan Perry 
 * Main takes the input file of text, number of leading zeros needed for success, 
 * and the number of processes to start. After error checking, a list of strings
 * is prepared to be sent to the ./hasher program so that each child process sends
 * a respective output file name. The program runs a loop, sending off all the 
 * children to start a hasher, and then upon return of first child, the rest are sent 
 * terminating signals and reaped. The correct output file and time taken to run 
 * the program are outputted to the user.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main(int argv, char** argc) {
    // error check valid number of inputs
    if (argv != 4) {
        fprintf(stderr, "Error: invalid number of inputs\n");
        exit(1);
    }

    // iterator
    int i;
    // input file name
    char* infile = argc[1];

    // open file to read
    FILE* pfile = fopen(infile, "r");
    // check if file pointer is null (indicating invalid file)
    if (pfile == NULL) {
        printf("\nerror: '%s' is an invalid file\n", infile);
        printf("ensure file exists and entered correctly\n\n");
        exit(-1);
    }
    // close file pointer
    fclose(pfile);

    // get number of processes to start
    int zeros = atoi(argc[2]);
    // number of processes to start
    int procs = atoi(argc[3]);
    // declare argChild array to pass into next function
    char* argChild[5];
    // NULL terminate the input array of strings
    argChild[4] = NULL;

    // allocate array to store all output file names
    char** outfileNames = malloc(procs * sizeof(char*));
    // allocate array to store all the pids from the forks
    pid_t* processIDs = malloc(procs * sizeof(pid_t));

    // start clock object
    struct timeval start, end;
    gettimeofday(&start, NULL);

    printf("\nRunning...\n");
    // start procs amount of processes to simulateous check nonces
    for (i = 0; i < procs; i++) {    
        
        // allocate memory for output file name and store pointer
        outfileNames[i] = malloc(20 * sizeof(char));
        sprintf(outfileNames[i], "out%d.txt", i); 

        // Create a new process and record pid
        pid_t pid = fork(); 

        // if the pid is less than 0, error flag, exit
        if (pid < 0) {
            printf("Fork failed\n");
            exit(-1);
        } 
        // else if it is a child process, start mining!
        else if (pid == 0) {
            // put in the porgram name for the execvp call
            argChild[0] = "./hasher";

            // add the input file name
            argChild[1] = infile;

            // store this string value in the cmd line arguments for this child process
            argChild[2] = outfileNames[i];

            // number of zeros that should be prepended for valid hashed value
            char zeroStr[5];
            sprintf(zeroStr, "%d", zeros);
            argChild[3] = zeroStr;

            // call execvp to create a hasher for this child process
            execvp(argChild[0], argChild);

            // throw error if child gets passed execvp
            fprintf(stderr, "Child error\texecvp did not work\n");
            return 1;
        } 
        // Parent process records all pids
        else {
            // record pid's 
            processIDs[i] = pid;
        }
    }
    
    // reap programs as they wake up and using wait, find which process just woke up
    pid_t procEnded;
    // &int param stores data related to process?
    procEnded = wait(NULL);
    // stop clock and calculate how long it has been
    gettimeofday(&end, NULL);

    // procEnded is terminated, so kill all other processes
    for (int i = 0; i < procs; i++) {
        if (procEnded != processIDs[i]) {
            kill(processIDs[i], SIGTERM);
            // printf("child index %d terminated\n", i);
        }
        // else, it was the first child to return so print out message
        else {
            printf("\nSuccessful block outfile: %*s\n", 10, outfileNames[i]);
            printf("Amount of leading zeros:    %d\n", zeros);
        }
        // free that bit of dynamically allocated memory
        free(outfileNames[i]);
    }

    // Calculate the elapsed time in seconds and microseconds
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    // If the end microseconds are smaller than the start microseconds, adjust the seconds
    if (microseconds < 0) {
        seconds--;
        microseconds += 1000000;
    }
    // print out the time summary
    printf("Total time for program was: %1.6f\n\n", seconds + microseconds/1000000.0);

    // free memory
    free(processIDs);
    free(outfileNames);

    return 0;
}