/* 
* Sources: 
*   - https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm 
*
*/

#include "utils.h"

void run_rr(int argc, char *argv[]) { 
    if (argc < 4) {     
        fprintf(stderr, "scheduling quantum required.\n");
        exit(-1);
    }

    if (argc > 4) { 
        fprintf(stderr, "exceeded the number of required arguments.\n");
        exit(-1);
    }

    const int BASE = 10;
    int quantum;
    char *endptr;
    
    quantum = strtol(argv[3], &endptr, BASE);

    if (endptr == argv[3]) { 
        fprintf(stderr, "Numerical value not found.\n");
        exit(-1);
    }

    if (quantum < 2 || quantum > 4) { 
        fprintf(stderr, "Quantum must fall within the valid range of 2-4ms.\n");
        exit(-1);
    }

    puts("\nRunning RR...");
    puts("Time | Running | Ready Queue");
    puts("-----------------------------");

    // some logic here

    puts("");


    puts("\nRR Execution Completed.");
    puts("Average Response Time:");
    puts("Average Turnaround Time:");
    puts("Total Context Switches:");


    puts("");
}

void run_fcfs(int argc, char *argv[]) { printf("%d\n", argc); }
