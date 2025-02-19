/* 
* Sources: 
*   - https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm 
*
*/

#include "utils.h"

/**
 *  @brief count the total number of processes.
 *  
 *  @param file_pointer the pointer to the text file
 * 
 *  @returns the total number of processes
 * 
 */
int count(FILE* file_pointer) { 
    char line[256];
    int numberOfProcesses = 0;
    while (fgets(line, sizeof(line), file_pointer)) { 
        numberOfProcesses++; 
    }
    return numberOfProcesses;
}

void run_rr(int argc, char *argv[]) {

    // error-handling: scheduling quantum 
    if (argc < 4) {     
        usage("Scheduling quantum required.");
    }

    if (argc > 4) { 
        usage("Exceeded the number of required arguments.");
    }

    const int BASE = 10;
    int quantum;
    char *endptr;
    
    quantum = strtol(argv[3], &endptr, BASE);

    if (endptr == argv[3]) { 
        usage("Numerical value not found.");
    }

    if (quantum < 1 || quantum > 5) { 
        usage("Quantum must fall within the valid range of 1-5ms.");
    }

    // error-handling: verifying the input file 
    FILE *fp = fopen(argv[2], "r");

    if (fp == NULL) { 
        usage("Failed to open file.");
    }

    // TODO: Implement the Ready Queue / Initialize the Structures
    // TODO: Determine the number of processes.
    int numbOfProcesses = count(fp); // NOTE TO SELF: this is working so far.

    // some experimental code
    Process* p1 = (Process*)malloc(sizeof(Process));
    if (p1 == NULL) { 
        fprintf(stderr, "Memory allocation failed.");
        exit(-1);
    }

    // initialize the process
    strcpy(p1->pid, "p1");
    p1->arrival_time = 0;
    p1->run_time = 5;



    puts("\nRunning RR...");
    puts("Time | Running | Ready Queue");
    puts("-----------------------------");

    // some logic here

    puts("");

    fclose(fp);
    puts("\nRR Execution Completed.");
    puts("Average Response Time:");
    puts("Average Turnaround Time:");
    puts("Total Context Switches:");


    puts("");
}

void run_fcfs(int argc, char *argv[]) { printf("%d\n", argc); }
