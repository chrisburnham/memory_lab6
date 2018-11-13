/*
 * memorySim.c
 *      Author: cburnham
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freelist.h"

FILE * fd;   // input file descriptor
bool m_done = false;

// typedef enum Scheduler
// {
// 	FCFS, // First Come First Serve
// 	SJF, // Shortest Job First
// 	SJFP // Shortest Job First Premption
// } Scheduler;

/*
 Function to determine if a job is arriving during this particular time step.
 Returns 0 if no job is arriving
 Returns the duration of the job that arrives if a job does arrive
 */
int jobArrives(int timestep)
{
	static int nextJobTime = -1;
	static int nextDuration;

	if (nextJobTime == -1)
	{
		if (fscanf(fd, "%d %d", &nextJobTime, &nextDuration) == EOF)
		{
			m_done = true;
		}
		// else have something to work with, so continue....
	}

	if (nextJobTime == timestep)
	{
		nextJobTime = -1;
		return nextDuration;
	}
	else // not up to nextJobTime yet
	{
		return 0;
	}
}

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
void doSimulation()
{

	while(!m_done)
	{
		bool alloc;
		int id;
		int size;

		Get_next_request(&alloc, &id, &size);

		if(m_done)
			return;
	}
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <FCFS|SJF|SJFP> <memoryFile>\n", argv[0]);
		exit(1);
	}

	char * filename = argv[2];
        fd = fopen(filename, "r");
	if (fd == NULL)
	{
		printf("Can't open input file: %s\n", filename);
		exit(1);
	}

	doSimulation();

	// if (strcmp(argv[1], "FCFS") == 0)
	// 	{
	// 	printf("Running simulation for first come first serve\n");
	// 	doSimulation(FCFS);
	// 	}
	// else if (strcmp(argv[1], "SJF") == 0)
	// 	{
	// 	printf("Running simulation for shortest job first\n");
	// 	doSimulation(SJF);
	// 	}
	// else if (strcmp(argv[1], "SJFP") == 0)
	// 	{
	// 	printf("Running simulation for shortest job first with premption\n");
	// 	doSimulation(SJFP);
	// 	}
	// else
	// 	printf("Oops, bad scheduling type: %s", argv[1]);

}

