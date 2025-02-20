/* 
* Sources: 
*   - https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm 
*   - https://www.tutorialspoint.com/cprogramming/c_error_handling.htm
*   - https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
*
*/

#include "utils.h"

/**
 *  @brief set the cursor back the beginning.
 *  
 *  @param file_pointer the pointer to the text file
 * 
 */
void file_rewind(FILE* file_pointer) { 
    fseek(file_pointer, 0, SEEK_END);
    rewind(file_pointer);
}

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

    // TODO: Implement and Initialize the Ready Queue
    
    // Determine the number of processes.
    int numbOfProcesses = count(fp); // NOTE TO SELF: this is working so far.


    // Validate the number of processes
    if (numbOfProcesses < 2 || numbOfProcesses > 5) { 
        fprintf(stderr, "The number of processes must fall within the valid range of 2-5.\n");
        exit(-1);
    }

    // Point back to the beginning of the file
    file_rewind(fp);

    // TODO: Initialize the processes
    Process* processes = (Process*)malloc(numbOfProcesses * sizeof(Process));
    if (processes == NULL) { 
        fprintf(stderr, "Memory allocation failed.");
        fclose(fp);
        exit(-1);
    }

    // initialize the process
    // read the file line-by-line
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s %d %d",
                processes[i].pid,
                &processes[i].arrival_time,
                &processes[i].run_time);
        i++;
    }

    puts("\nRunning RR...");
    puts("Time | Running | Ready Queue");
    puts("-----------------------------");

    // some logic here

    for(int i = 0; i < numbOfProcesses; i++) { 
        printf("%s %d %d\n", processes[i].pid, 
            processes[i].arrival_time, 
            processes[i].run_time);
    }

    // enqueue the processes to the Ready Queue
    






    puts("");

    fclose(fp);
    puts("\nRR Execution Completed.");
    puts("Average Response Time:");
    puts("Average Turnaround Time:");
    puts("Total Context Switches:");


    puts("");
}

void run_fcfs(int argc, char *argv[]) { printf("%d\n", argc); }
