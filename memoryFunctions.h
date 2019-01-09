
#include <stdio.h>
#include "Queue.h"
#define MEMSIZE 128

typedef struct Memory
{
    int * position; //contains all the 1's and 0's that represent the memory slots
    //if 1, indicates a full slot, if 0, indicates an empty slot
    
    int numProcesses;
    int MAXSIZE; //max amount of memory
    int size;   //current amount filled of the MAXSIZE
    double * memUsage;
    int numAverages;
} Memory;

//PURPOSE:: clears every position in memory
//ARGUMENTS:: memory
void clearMem(Memory * memory);

//PURPOSE:: initializes memory
//ARGUMENTS:: memory
Memory * initializeMem (Memory * memory);

//PURPOSE:: finds the number of holes in memory
//whenever: it switches from a 1 to a 0 or a 0 to a 1, indicates a hole
//ARGUMENTS:: int array for memory
int holes (int * memList);

//PURPOSE:: based on the mode selected by the argument "type"
//1 = firstFit, 2 = bestFit, then select the proper fit (based on type given)
//and return its position in memory
//
//ARGUMENTS:: the memory, the requiredSize it has to be, and the type of fit
//required.
int fit (Memory * memory, int requiredSize, int type);

//PURPOSE:: checks if there is enough space to store the process
//ARGUMENTS:: the memory, and the size required to find
//RETURNS:: 0 if unable to locate the required size of memory units
//Otherwise, will return position / requiredSize.
int isThereEnoughMemory (Memory * memory, int requiredSize);

//PURPOSE:: adds the amount of memory as determined by the Head of the processes queue
//ARGUMENTS:: the position to add from, the memory, and the processes queue
void addToMem(int position, Memory * memory, Queue * processes);

//PURPOSE:: removes the amount of memory indicated by the head of the processes queue
//ARGUMENTS:: the memory, and the processes queue
void removeFromMem(Memory * memory, Queue * processes);

//PURPOSE:: frees all memory used in holes.c
//ARGUMENTS:: memory data type, char array of ID's, int array of values,
//ready Q, and process Q
void freeAllMem(Memory * memory, char * processID, int * values, Queue * ready,
    Queue * processes);
