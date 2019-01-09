/*contains all code I used to manage the queues I used*/
//A queue in my program being a circular array sequential queue

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

Process * createProcess(char ID, int size, int swaps)
{
    Process * new;
    new = malloc(sizeof(Process));
    new->ID = ID;
    new->size = size;
    new->swaps = swaps;

    return new;
}

Queue * initQueue(Queue * Q, int size)
{
    int i;
    Q = malloc(sizeof(Queue));
    Q->head = 0;
    Q->tail = -1;
    Q->process = malloc(sizeof(Process *) * size);
    for(i=0; i < size; i++)
        Q->process[i] = malloc(sizeof(Process));
    
    Q->MAXSIZE = size;
    return Q;
}

//I used pre-increment, so that the tail will be at 0 after initialization,
//and will have a value there, and the tail will be located at 0.
//Before this, it would advance one too many with post-increment. 
//which is why tail is initialized to -1.
void Add(Queue * Q, Process * toAdd)
{
    Q->process[++Q->tail] = toAdd;

    if(Q->tail == Q->MAXSIZE)
        Q->tail = 0;
}

void Dequeue(Queue * Q)
{
    Q->process[Q->head]->size = Q->process[Q->head]->swaps = 0;
    Q->process[Q->head++]->ID = ' ';
    
    if(Q->head == Q->MAXSIZE)
        Q->head = 0;
}

Process * Head(Queue * Q)
{
    return Q->process[Q->head];
}

int isEmpty(Queue * Q)
{
    int i;
    for(i=0; i < Q->MAXSIZE; i++)
        if(Q->process[i]->ID != ' ' && Q->process[i]->size != 0)
            return 0;

    return 1;
}

Queue * setValues(char * processID, int * values, int size, Queue * ready)
{
    int i;
    for(i=0; i < size; i++)
    {
        Add(ready, createProcess(processID[i], values[i], 0));
        //add each process to the ready Queue
    }
    return ready;
}

void makeEmpty(Queue * Q)
{
    int i;
    Q->tail = -1; 
    Q->head = 0;
    for(i=0; i < Q->MAXSIZE; i++)
    {
        Q->process[i]->ID = ' ';
        Q->process[i]->size = 0;
        Q->process[i]->swaps = 0;
    }
}

void freeQueue(Queue * Q)
{
    int i;
    for(i=0; i < Q->MAXSIZE; i++)
    {
        free(Q->process[i]);
    }
    free(Q->process);
    free(Q);
}