/* 
* Sources: 
*   - https://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm 
*   - https://www.tutorialspoint.com/cprogramming/c_error_handling.htm
*   - https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
*   - https://www.geeksforgeeks.org/c-program-to-implement-circular-queue/
*
*/

#include "utils.h"

int size = 0; // used to keep track of the number of elements in the queue
int rear = -1, front = -1;

/**
 *  @brief check if the queue is full.
 */
int isFull(int size) { 
    // if the next position is the front, the queue is full
    return (rear + 1) % size == front;
}

/**
 *  @brief check if the queue is empty.
 */
int isEmpty() { 
    // if the front hasn't been set, the queue is empty
    return front == -1;
}

/**
 *  @brief insert an element to the queue
 * 
 *  @param queue the queue to operate on
 *  @param process the process to insert into the queue
 */
void enqueue(char* queue[], char* process, int size) {

    // check if the queue is full before inserting
    if (isFull(size)) { 
        puts("the queue is full.");
        return;
    }

    // if the queue is empty, set the front to the first position
    if (front == -1) { 
        front = 0;
    }

    rear = (rear + 1) % size;
    queue[rear] = process;
}

/**
 *  @brief remove an element from the queue.
 * 
 *  @param queue the queue to operate on
 */
char* dequeue(char* queue[], int size) { 
    if (isEmpty()) { 
        puts("the queue is empty.");
        exit(-1);
    }

    char* process = queue[front];

    if (front == rear) { 
        front = rear = -1;
    } else { 
        front = (front + 1) % size;
    }
    
    return process;
}

/**
 *  @brief display the elements of the queue.
 *  
 *  @param queue the queue to operate on
 * 
 */
void display(char* queue[], int max_size) { 
    if (isEmpty()) { 
        puts("-");
        return;
    }
    int i = front;
    while (i != rear) { 
        printf("%s ", queue[i]);
        i = (i + 1) % max_size;
    }
    printf("%s\n", queue[rear]);
}

int isInQueue(char* process, char* queue[], int max_size) { 
    for (int i = 0; i < max_size; i++) { 
        if (strcmp(queue[i], process) == 0) { 
            return 1;
        }
    }
    return 0;
}

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
    
    const int BASE = 10;
    char *endptr;
    int quantum;
    
    // error-handling: scheduling quantum 
    if (argc < 4) { usage("Scheduling quantum required."); }
    if (argc > 4) { usage("Exceeded the number of required arguments."); }

    quantum = strtol(argv[3], &endptr, BASE);

    if (endptr == argv[3]) { usage("Numerical value not found."); }
    if (quantum < 1 || quantum > 5) { usage("Quantum must fall within the valid range of 1-5ms."); }

    // error-handling: verifying the input file 
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL) { usage("Failed to open file."); }

    
    // Determine the number of processes.
    int numbOfProcesses = count(fp);

    // Validate the number of processes
    if (numbOfProcesses < 2 || numbOfProcesses > 5) { 
        fprintf(stderr, "The number of processes must fall within the valid range of 2-5.\n");
        exit(-1);
    }

    // Point back to the beginning of the file
    file_rewind(fp);

    // Allocate space for storing processes
    Process* processes = (Process*)malloc(numbOfProcesses * sizeof(Process));
    if (processes == NULL) { 
        fprintf(stderr, "Memory allocation failed.");
        fclose(fp);
        exit(-1);
    }

    // Initialize the processes
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s %d %d",
                processes[i].pid,
                &processes[i].arrival_time,
                &processes[i].run_time);
        i++;
    }

    // Point back to the beginning of the file
    file_rewind(fp);

    // Intialize the queue 
    char* queue[numbOfProcesses + 1];

    // insert the process that arrives first.
    for (int i = 0; i < numbOfProcesses; i++) { 
        // check which is the first one
        if (processes[i].arrival_time == 0) {
            enqueue(queue, processes[i].pid, numbOfProcesses);
        }
    }

    puts("\nRunning RR...");
    puts("Time | Running  | Ready Queue");
    puts("-----------------------------");

    int timer = 0;
    char* running_process = NULL;
    while (!isEmpty()) { 

        printf("%d-%d  |", timer, timer + quantum);
        running_process = dequeue(queue, numbOfProcesses);
        printf("%s        |", running_process);
        timer += quantum;

        // reduce the runtime of the running process
        for (int i = 0; i < numbOfProcesses; i++) { 
            if (strcmp(processes[i].pid, running_process) == 0) { 
                processes[i].run_time -= quantum;
                break;
            }
        }

        for (int i = 0; i < numbOfProcesses; i++) { 
            if (strcmp(processes[i].pid, running_process) != 0 && processes[i].arrival_time <= timer && processes[i].run_time > 0) {
                if (!isInQueue(processes[i].pid, queue, numbOfProcesses)) { 
                    enqueue(queue, processes[i].pid, numbOfProcesses);
                }
            }
        }

        //diplay the current queue
        display(queue, numbOfProcesses);
    }

    puts("");

    fclose(fp);
    free(processes);
    puts("\nRR Execution Completed.");
    puts("Average Response Time:");
    puts("Average Turnaround Time:");
    puts("Total Context Switches:");

    puts("");
}

void run_fcfs(int argc, char *argv[]) { 
    
    // error-handling: scheduling quantum 
    if (argc > 3) { usage("Exceeded the number of required arguments."); }

    // error-handling: verifying the input file 
    FILE *fp = fopen(argv[2], "r");
    if (fp == NULL) { usage("Failed to open file."); }

    
    // Determine the number of processes.
    int numbOfProcesses = count(fp);

    // Validate the number of processes
    if (numbOfProcesses < 2 || numbOfProcesses > 5) { 
        fprintf(stderr, "The number of processes must fall within the valid range of 2-5.\n");
        exit(-1);
    }

    // Point back to the beginning of the file
    file_rewind(fp);

    // Allocate space for storing processes
    Process* processes = (Process*)malloc(numbOfProcesses * sizeof(Process));
    if (processes == NULL) { 
        fprintf(stderr, "Memory allocation failed.");
        fclose(fp);
        exit(-1);
    }

    // Initialize the processes
    int i = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s %d %d",
                processes[i].pid,
                &processes[i].arrival_time,
                &processes[i].run_time);
        i++;
    }

    // Point back to the beginning of the file
    file_rewind(fp);

    // Intialize the queue 
    char* queue[numbOfProcesses + 1];

    // insert the processes into the queue
    for (int i = 0; i < numbOfProcesses; i++) { 
        enqueue(queue, processes[i].pid, numbOfProcesses);
    }

    // sort the queue based on arrival time
    for (int i = 0; i < numbOfProcesses - 1; i++) { 
        for (int j = 0; j < numbOfProcesses - i - 1; j++) { 
            if (processes[i].arrival_time > processes[j + 1].arrival_time) { 
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int timer = 0;
    int complete = 0;
    int contextSwitches = 0;
    char* running_process = NULL;
    char* current_process = NULL;
    puts("\nRunning RR...");
    puts("Time | Running  | Ready Queue");
    puts("-----------------------------");

    while (!isEmpty()) { 
        



    puts("");

    fclose(fp);
    free(processes);
    puts("\nRR Execution Completed.");
    puts("Average Response Time:");
    puts("Average Turnaround Time:");
    puts("Total Context Switches:");

    puts("");

}
