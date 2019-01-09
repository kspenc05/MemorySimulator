#include <stdio.h>

//The elements stored in the Queue (Processes)
typedef struct Process
{
    char ID;
    int size;
    int swaps; //times a process has been swapped
}Process;

//The Queue data type
typedef struct Queue
{
    Process ** process;
    int tail;
    int head;
    int MAXSIZE;
}Queue;

//PURPOSE:: initializes a queue
//ARGUMENTS:: the queue pointer (NULL), and the size it needs
//RETURNS:: the newly created queue
Queue * initQueue(Queue * Q, int size);

//PURPOSE:: creates a process to store in Queue based on arguments
//ARGUMENTS:: The process ID, and the process size
//RETURNS:: the newly created process
Process * createProcess(char ID, int size, int swaps);

//PURPOSE:: adds element to Queue
//ARGUMENTS:: Queue and element to Add
void Add(Queue * Q, Process * toAdd);

//PURPOSE:: removes element from Queue
//ARGUMENTS:: Queue
void Dequeue(Queue * Q);

//PURPOSE:: returns head to Queue
//ARGUMENTS:: Queue
//Returns:: Head of Queue
Process * Head(Queue * Q);

//PURPOSE:: sets the queue's values with two arrays
//ARGUMENTS:: set of character ID's (each 1 character), set of sizes for each element,
//size of the queue (what its MAXSIZE will be), and the Queue itself
Queue * setValues(char * processID, int * values, int size, Queue * ready);

//PURPOSE:: clears all the data in the Queue so it can be re-used again
//ARGUMENTS:: the queue to be cleared
void makeEmpty(Queue * Q);

//PURPOSE:: checks if empty, returns 1 if empty, 0 if not empty
//ARGUMENTS:: the Queue in question
int isEmpty(Queue * Q);

//PURPOSE:: frees Queue
//ARGUMENTS:: Queue
void freeQueue(Queue * Q);