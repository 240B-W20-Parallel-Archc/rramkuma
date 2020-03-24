/*
 * cacheperf.c
 *
 *  Created on: Mar 12, 2020
 *      Author: ranga.ramkumar
 */

#include "cacheperf.h"
#include <stdio.h>

int thread_count;
int array_size;
bool random_flag;
bool seq_flag;
bool write_flag;

//function to figure out time
//function for random number generator? - within array access
//

int performance() {
	printf("Hello world\n");
	printf("thread count is %d \n", thread_count);
	printf("array size is %d bytes\n", array_size);
	printf("random flag is %d\n", random_flag);
	/*
	 * Below, do cache accesses, return latency for each access.
	 * Aim is to do read, rmw for single thread and same for multiple threads
	 *
	 * For read: init array, read randomly from a few spaces
	 * Divide total time taken by count to get average access time
	 * Use random number generator for random access
	 * Measure time diff between two to see access time for reading from cache
	 */

	return 0;
}
