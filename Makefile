
holes: Queue.o memoryFunctions.o
	gcc -Wall -std=c99 -pedantic holes.c Queue.o memoryFunctions.o -o holes 

Queue.o: 
	gcc -c -Wall -std=c99 -pedantic Queue.c 
    
memoryFunctions.o:
	gcc -c -Wall -std=c99 -pedantic memoryFunctions.c

clean:
	rm holes *.o