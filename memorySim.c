/*
 * schedulingSim.c
 *
 *  Updated on: Oct 2, 2018
 *      Author: ldamon
 *  Updated by cburnham
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

FILE * fd;   // input file descriptor
bool done = false;

typedef enum Scheduler
{
	FCFS, // First Come First Serve
	SJF, // Shortest Job First
	SJFP // Shortest Job First Premption
} Scheduler;

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
			done = true;
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
void doSimulation(Scheduler sched)
{
	bool debug_prints = false;

	int current_time = 0;
	Job current_job;
	bool running = false;
	int total_jobs = 0;
	int max_queue_length = 0;
	int total_run_time = 0;
	int total_wait_time = 0;

	current_job.duration = 0;
	current_job.waitTime = 0;

	while(!done || (queueLength() > 0) || running)
	{
		if(!done)
		{
			// Check if we have anything new
			const int new_job_duration = jobArrives(current_time);
			if(new_job_duration != 0)
			{
				if(debug_prints)
				{
					printf("adding new job with %i duration\n", new_job_duration);
				}

				Job new_job;
				new_job.duration = new_job_duration;
				new_job.waitTime = 0;
				queueAdd(new_job);
				total_jobs++;
			}
		}

		// Get our next job to run
		if(!running && queueLength() > 0)
		{
			switch(sched)
			{
				case FCFS:
					current_job = queueRemoveFirst();
					break;
				case SJF:
					current_job = queueRemoveShortest();
					break;
				case SJFP:
					current_job = queueRemoveShortest();
					break;
			}

			if(current_job.duration > 0)
			{
				running = true;
			}
		}

		// Run a job for the time unit
		if(running)
		{
			if(sched == SJFP && (queueLength() > 0))
			{
				Job shortest_queued = queueRemoveShortest();
				if(shortest_queued.duration < current_job.duration)
				{
					if(debug_prints)
					{
						printf("Prempting job\n");
					}
					queueAdd(current_job);
					current_job = shortest_queued;
				}
				else
				{
					queueAdd(shortest_queued);
				}
			}

			total_run_time++;
			current_job.duration--;
			if(current_job.duration <=0)
			{
				total_wait_time += current_job.waitTime;
				running = false;
			}
		}

		const int queue_len = queueLength();
		if(queue_len > max_queue_length)
		{
			max_queue_length = queue_len;
		}

		if(debug_prints)
		{
			printf("At time %i, running a job with %i remaining\n", 
				     current_time, current_job.duration);
			printf("Current Queue:\n");
			queuePrint(); 
			printf("\n");
		}

		queueIncrementWaitTimes();
		current_time++;
	}

	printf("Ran for %i time units. Ran %i jobs. Longest queue length was %i\n", 
				 current_time, total_jobs, max_queue_length);
	printf("Total runtime: %i. Total wait time: %i\n", 
		     total_run_time, total_wait_time);
	printf("Average wait time: %f\n", 
		    ((double)total_wait_time / (double)total_jobs));
}


int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <FCFS|SJF|SJFP> <scheduleFile>\n", argv[0]);
		exit(1);
	}

	char * filename = argv[2];
        fd = fopen(filename, "r");
	if (fd == NULL)
	{
		printf("Can't open input file: %s\n", filename);
		exit(1);
	}

	if (strcmp(argv[1], "FCFS") == 0)
		{
		printf("Running simulation for first come first serve\n");
		doSimulation(FCFS);
		}
	else if (strcmp(argv[1], "SJF") == 0)
		{
		printf("Running simulation for shortest job first\n");
		doSimulation(SJF);
		}
	else if (strcmp(argv[1], "SJFP") == 0)
		{
		printf("Running simulation for shortest job first with premption\n");
		doSimulation(SJFP);
		}
	else
		printf("Oops, bad scheduling type: %s", argv[1]);

}

