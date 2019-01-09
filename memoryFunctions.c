/*contains all code to do with managing the memory structure*/
//as defined in memoryFunctions.c, where all the function header comments are 
//The actual 'memory slots' used in this program, consist of just an int
//array with 1's and 0's, 1 = full, 0 = empty

#include <stdio.h>
#include <stdlib.h>
#include "memoryFunctions.h"

void clearMem(Memory * memory)
{
    int i;
    for(i=0; i < MEMSIZE; i++)
        memory->position[i] = 0;
    
    memory->numProcesses = memory->size = memory->numAverages = 0;
    memory->MAXSIZE = MEMSIZE;
}

Memory * initializeMem (Memory * memory)
{
    memory = malloc(sizeof(Memory));
    memory->position = malloc(sizeof(int) * MEMSIZE);
    memory->memUsage = malloc(sizeof(double));
    
    clearMem(memory);
    return memory;
}

int holes (int * memList)
{
    int i, holes = 0;
    
    if(memList[0] == 0)
        holes++;
    
    for(i=0; i < MEMSIZE; i++)
    {
        if(memList[i+1] == 0 && memList[i] == 1)
            holes++;
    }
    return holes;
}

int isThereEnoughMemory (Memory * memory, int requiredSize)
{
    int i, spacesInMem = 0;
    for(i=0; i < memory->MAXSIZE+1; i++)
    {
        if(memory->position[i] != 0)
            spacesInMem = 0;
            
        if(spacesInMem >= requiredSize)
            return 1;
            
        spacesInMem++;
    }
    return 0;
}

//NOTE: the types of fit modes are as follows for fit():
// 1 - first fit, 2 - best fit, 3 - worst fit, 4 - next fit.
int fit (Memory * memory, int requiredSize, int type)
{
    int position, freeSpaces, * positions, *fits, fitsFound, i, j, start, begin;
    static int next = 0;
    fits = positions = NULL;
    
    if(type == 4)
    {
        if(next + requiredSize > MEMSIZE)
            next = 0;
        begin = next;
    }
    else
        begin = 0;
        
    position = freeSpaces = fitsFound = start = 0;
    
    fits = malloc(sizeof(int));
    positions = malloc(sizeof(int));
    for(i=begin; i < memory->MAXSIZE; i++)
    {
        if(memory->position[i] == 0)
            freeSpaces++;
        else
        {
            start = position+1; //start will contain the location after the last found '1'
            freeSpaces = 0;
        }
        if(type == 1 || type == 4)
        {
            if(freeSpaces == requiredSize)
            {
                free(positions);
                free(fits);
                if(type == 4)
                    next = start;
                
                return start;
            }
        }
        
        else if(type == 2 || type == 3)
        {
            if(freeSpaces >= requiredSize)
            {
                fits = realloc(fits, sizeof(int) * (fitsFound + 1));
                positions = realloc(positions, sizeof(int) * (fitsFound + 1));
                
                positions[fitsFound] = position;
                fits[fitsFound++] = freeSpaces;
            }
        }
        position++;
    }
    
    for(i=0; i < 1; i++) //only occurs for best or worst
    {
        for(j=0; j < fitsFound; j++)
        {
            if(fits[i] > fits[j] && type == 2)
            {
                fits[i] = fits[j]; //gets smallest size
                positions[i] = positions[j];
            }
            else if(fits[i] < fits[j] && type == 3)
            {
               fits[i] = fits[j]; //gets biggest
               positions[i] = positions[j];
            }
        }
    }
    start = positions[0] - fits[0]; //since positions are incremented at the same time
    free(positions);  
    free(fits);
    
    if(start < 0)
        return 0;
    else
        return start++;
}

//PURPOSE:: determines cumulative memory usage and returns it
//ARGUMENTS:: memory unit 
double getUsageAverage(Memory * memory)
{
    int i;
    double cumulative = 0;
    for(i=0; i < memory->numAverages; i++)
    {
        cumulative = cumulative + memory->memUsage[i]; 
    }
    return cumulative / (double) memory->numAverages;
}

//adds 1 process to memory
void addToMem (int position, Memory * memory, Queue * ready)
{
    int i;
    for(i=position;  i < position + Head(ready)->size; i++)
    {
        memory->position[i] = 1;
        memory->size++;
    }
    
    memory->memUsage = realloc(memory->memUsage, sizeof(double) * (memory->numAverages +1));
    
    memory->memUsage[memory->numAverages] = ( (double) memory->size / memory->MAXSIZE) * 100;
    
    printf("pid loaded, #processes = %d, #holes = %d, %%memUsage = %.0lf, ",
        ++memory->numProcesses, holes(memory->position), 
        memory->memUsage[memory->numAverages++]);
        
    printf("cumulative %%mem = %.0lf\n", getUsageAverage(memory));
}

//removes 1 process from memory
void removeFromMem(Memory * memory, Queue * processes)
{
    int start, i = 0;
    
    while(i++ < memory->MAXSIZE && memory->position[i] == 0);

    start = i;
    for(i=start; i < Head(processes)->size + start; i++)
    {
        memory->position[i] = 0;
        memory->size--;
    }
    if(memory->numProcesses - 1 >=0) //process number must be nonnegative
        memory->numProcesses--; 
}

void freeMem(Memory * memory)
{
    free(memory->position);
    free(memory->memUsage);
    free(memory);
}

void freeAllMem(Memory * memory, char * processID, int * values, Queue * ready,
    Queue * processes)
{
    free(processID);
    free(values);
}