/*
 * memorySim.c
 *      Author: cburnham
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freelist.h"

#define MAXSIZE 10000

FILE * fd;   // input file descriptor
bool m_done = false;

typedef enum Alloc_strategy
{
	FIRST, 
	BEST,  
	WORST  
} Alloc_strategy;

// Gets the next request from our file
// alloc is true if we are allocating and false for a free
// id is the id of the memory block
// size is the requested size (only applicable for alloc)
void Get_next_request(bool* alloc, int* id, int* size)
{
	char type[10];

	if(fscanf(fd, "%s %d %d", type, id, size) == EOF)
	{
		m_done = true;
	}

	if(strcmp(type, "ALLOC") == 0)
	{
		*alloc = true;
	}
	else if(strcmp(type, "FREE") == 0)
	{
		*alloc = false;
	}
	// else??
}

// TODO doc
Node* Find_first(int size)
{
	Node* nd = getFirst();

	while(nd != NULL)
	{
		if(nd->block.size >= size)
			break;

		nd = nd->next;
	}

	return nd;
}

// TODO doc
Node* Find_best(int size)
{
	Node* nd = getFirst();
	Node* best = NULL;

	while(nd != NULL)
	{
		if(nd->block.size >= size)
		{
			if((best == NULL) || (nd->block.size < best->block.size))
				best = nd;
		}

		nd = nd->next;
	}

	return best;
}

// TODO doc
Node* Find_worst(int size)
{
	Node* nd = getFirst();
	Node* worst = NULL;

	while(nd != NULL)
	{
		if(nd->block.size >= size)
		{
			if((worst == NULL) || (nd->block.size > worst->block.size))
				worst = nd;
		}

		nd = nd->next;
	}

	return worst;
}

/*
 This is the main function of the simulation.   It should move the time along step by step,
 calling jobArrives(timestep) for each step in time.  If a job arrives, it should be added
 to the job queue.
 
 Additionally, during each time step, the processor should be kept busy if possible.  If it
 is not already busy, a job should be removed from the queue and should start to run.  If a
 job is already running, then the amount of time remaining should be decremented.  You'll also
 need to be sure to update the wait time on any jobs in the queue.
 
 There is a global variable, done, which will be set when the entire input file has
 been read and the last job has been returned to the simulation.  You can use that variable to
 control when your simulation should end.
 
 INPUT sched:  enum  SJF means it should run a shortest job first simulation
                     FCFS means it should run a first come first serve simulation
                     SJFP means it should run a shortest job first simulation with premption
 
 OUTPUT: void, but the function prints out info about the jobs run
 */
// TODO: fix doc
void doSimulation(Alloc_strategy stg)
{
	MemBlock allocated[MAXSIZE];
	int addr = 0;
	bool debug = true;


	// TODO: This should go into the loop whenever we are out
	MemBlock initial;

	initial.size = BLOCK_SIZE;
	initial.startingAddr = addr;
	// addr += BLOCKSIZE
	insertFirst(initial);


	while(!m_done)
	{
		bool alloc;
		int id;
		int size;

		Get_next_request(&alloc, &id, &size);

		if(m_done)
			return;

		if(debug)
			printf("%s %i %i\n", alloc ? "ALLOC" : "FREE", id, size);

		if(alloc)
		{
			Node* nd;

			switch(stg)
			{
				case FIRST:
					nd = Find_first(size);
					break;

				case BEST:
					nd = Find_best(size);
					break;

				case WORST:
				  nd = Find_worst(size);
					break;
			}

			// TODO: if node == NULL add another block
			if(nd == NULL)
			{
				printf("we need to impliment adding blocks\n");
			}

			allocated[id].startingAddr = nd->block.startingAddr;
			allocated[id].size = size;

			if(size != nd->block.size)
			{
				MemBlock new_block;
				new_block.startingAddr = nd->block.startingAddr + size;
				new_block.size = nd->block.size - size;
				insertAfter(new_block, nd);
			}

			deleteNode(nd);
		}
		else // free
		{
			Node* nd = getFirst();
			bool inserted = false;

			do
			{
				if(nd->block.startingAddr > allocated[id].startingAddr)
				{
					insertAfter(allocated[id], nd->prev);
					inserted = true;
					break;
				}
			}
			while((nd = nd->next) != NULL);

			if(!inserted)
				insertAfter(allocated[id], getLast());

			allocated[id].size = 0;

			// TODO: coalesce
		}

		if(debug)
		{
			printf("free list:\n");
			Node* nd = getFirst();

			while(nd != NULL)
			{
				printf("  start %i, size %i\n", nd->block.startingAddr, nd->block.size);

				nd = nd->next;
			}
		}
	}
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <FIRST|BEST|WORST> <memoryFile>\n", argv[0]);
		exit(1);
	}

	char * filename = argv[2];
        fd = fopen(filename, "r");
	if (fd == NULL)
	{
		printf("Can't open input file: %s\n", filename);
		exit(1);
	}

	if (strcmp(argv[1], "FIRST") == 0)
	{
		printf("Running simulation for allocating first\n");
		doSimulation(FIRST);
	}
	else if (strcmp(argv[1], "BEST") == 0)
	{
		printf("Running simulation for allocating best\n");
		doSimulation(BEST);
	}
	else if (strcmp(argv[1], "WORST") == 0)
	{
		printf("Running simulation for allocating worst\n");
		doSimulation(WORST);
	}
	else
		printf("Oops, bad scheduling type: %s", argv[1]);
}

