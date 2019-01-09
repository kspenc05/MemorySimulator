/*contains all functions that only have to with running the simulation
or helping run the simulation*/
#include <stdio.h>
#include <stdlib.h>
#include "memoryFunctions.h"

//PURPOSE:: reads contents of file
//ARGUMENTS:: FILE pointer, int array of values, char array of processID's, and
//a counter variablle for number of items in file
void readFile(FILE * file, int * values, char * processID, int * counter)
{
    char buffer[50];
    while(fgets(buffer, 50, file) != NULL)
    {
        processID = realloc(processID, sizeof(char) * *counter+1);
        values = realloc(values, sizeof(int) * *counter+1);
        sscanf(buffer, "%c %d", &processID[*counter], &values[*counter]);
        *counter = *counter + 1;
    }
}

//PURPOSE:: is used to find average processes, and averageHoles. calculates average
//for an array of doubles
//ARGUMENTS:: double array and number of doubles
//returns the average
double getAverage(double * averageProcesses, int loads)
{
    int i;
    double total = 0;
    for(i=0; i < loads; i++)
        total = averageProcesses[i] + total;
    
    return total / loads;
}

//PURPOSE:: prints the totals after each allocation strategy is completed
//ARGUMENTS:: number of loads, double pointer to averageProcesses, averageHoles,
//and pointer to memory data type.
void printTotals (int loads, double * averageProcesses, double * averageHoles,
    Memory * memory)
{
    printf("\n\n\nTotal loads = %d, average #processes = %.1f, ", 
        loads, getAverage(averageProcesses, loads)); 
    
    printf("average #holes = %.1f ",  getAverage(averageHoles, loads)); 

    printf("cumulative %%mem = %.0lf\n\n\n", getAverage( (double *) memory->memUsage, 
    memory->numAverages));
}    

//PURPOSE:: makes two queues and a memory unit empty, clears all values, but does
//not free any memory.
//ARGUMENTS:: two queues, and a memory unit
void clearDataStructures(Queue * ready, Queue * processes, Memory * memory)
{
    clearMem(memory);
    makeEmpty(processes); 
    makeEmpty(ready);
}

//PURPOSE:: runs through one simulation cycle. Based on fitType, will run that 
//allocating strategy (eg. fitType == 1, perform first fit)
//ARGUMENTS:: the ready queue, the processes queue, the memory data type,
//and the fitType the caller wishes to run.
void runSimulation (Queue * ready, Queue * processes, Memory * memory, int fitType)
{
    int loads = 0;
    double * averageProcesses, * averageHoles;
    averageProcesses = malloc(sizeof(double));
    averageHoles = malloc(sizeof(double));
    
    while(isEmpty(ready) == 0) //runs until nothing is in ready
    {
        if(isThereEnoughMemory(memory, Head(ready)->size) == 0 )
        {
            removeFromMem(memory, processes);
            Head(processes)->swaps++;
            
            if(Head(processes)->swaps < 3)
            {
                Add(ready, createProcess(Head(processes)->ID, 
                    Head(processes)->size, Head(processes)->swaps));
            }
            Dequeue(processes);
        }
        else if(isThereEnoughMemory(memory, Head(ready)->size) != 0)
        {
            //adds to memory based on int position returned by fit()
            addToMem(fit(memory, Head(ready)->size, fitType), memory, ready);
            
            Add(processes, createProcess( Head(ready)->ID, 
                Head(ready)->size, Head(ready)->swaps));
            
            Dequeue(ready);
            
            averageProcesses = realloc(averageProcesses, sizeof(double) * (loads + 1));
            averageHoles = realloc(averageHoles, sizeof(double) * (loads + 1));
            
            averageProcesses[loads] = memory->numProcesses;
            averageHoles[loads++] = holes(memory->position);
        
        }
    }
    printTotals(loads, averageProcesses, averageHoles, memory);
    
    free(averageProcesses);
    free(averageHoles);
    
    clearDataStructures(ready, processes, memory);
}

//PURPOSE:: this program will attempt to use several allocation strategies to
//show how holes can occur as memory is allocated by the CPU, without actually using
//real-time processes but based on the information a process would typically have.
int main(int argc, char ** argv)
{
    FILE * file;
    char * processID;
    char * allocationName [] = {"First Fit", "Best Fit", "Worst Fit", "Next fit"};
    int * values, counter = 0, i;
    Memory * memory;
    Queue * ready, * processes;
    processes = ready = NULL; 
    memory = NULL;
    
    if(argc != 2)
        exit(EXIT_FAILURE);
    
    if((file = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "error: file \"%s\" could not be opened\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    processID = malloc(sizeof(char));
    values = malloc(sizeof(int));
    
    readFile(file, values, processID, &counter);
    
    processes = initQueue(processes, counter);
    ready = initQueue(ready, counter);
    memory = initializeMem(memory);
    
    for(i=0; i < 4; i++)
    {
        ready = setValues(processID, values, counter, ready); //sets file values
        printf("\n%s\n\n\n", allocationName[i]); 
        runSimulation(ready, processes, memory, i+1);
    }
    
    freeAllMem(memory, processID, values, ready, processes);
    fclose(file);
    return 0;
}