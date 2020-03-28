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
int stride;
double array_size_in_kb;


struct Node {
	int data;
	struct Node* nextptr;
};

void randomize_ll(struct Node **node_list) {
	int len = (array_size)/(sizeof(int));
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < len - 1; i++) {
				int swap_idx = rand() % (len - 1);
				struct Node* tmp;
				tmp = node_list[i]->nextptr;
				node_list[i]->nextptr = node_list[swap_idx]->nextptr;
				node_list[swap_idx]->nextptr = tmp;
			}
	}
	node_list[len-1]->nextptr = node_list[0];
}

void sequential_ll(struct Node **node_list, int stride) {
	int len = (array_size)/(sizeof(int));
	for (int i = 0; i < len - 1; i++) {
			node_list[i]->nextptr = node_list[i+stride-1]->nextptr;
	}
	node_list[len-1]->nextptr = node_list[0];
}


/*
	 * Below, do cache accesses, return latency for each access.
	 * Aim is to do read, rmw for single thread and same for multiple threads
	 *
	 * For read: init array, read randomly from a few spaces
	 * Divide total time taken by count to get average access time
	 * Use random number generator for random access
	 * Measure time diff between two to see access time for reading from cache
	 */


void performance_latency() {

	FILE *fp;
	fp = fopen("output.csv", "a+");

	// linked list using malloc
	// Our accesses will be to this linked list
	int len = (array_size)/(sizeof(int));
	struct Node* memallocated = (struct Node*)malloc(len*sizeof(struct Node));
	struct Node **node_list = (struct Node**)malloc(len*sizeof(struct Node));
	for (int i = 0; i < len; ++i) {
		node_list[i] = &memallocated[i];
	}

	for(int i = 0; i < len - 1; i++) {
		node_list[i]->data = i;
		node_list[i]->nextptr = node_list[i+1];
	}
	node_list[len-1]->data=len-1;
	node_list[len-1]->nextptr=node_list[0];


	if(random_flag) {
		//printf("Entering random loop\n");
		randomize_ll(node_list);
	} else {
		sequential_ll(node_list, stride);
	}

	if(!write_flag) {
		//printf("entering plain read case loop\n");
		clock_t start, end;
		double cpu_time_in_ns;
		int read_count = 0;
		int loop_count = len;
		start = clock();
		struct Node* nextnode = node_list[1];
		while (loop_count > 0) {
			nextnode = nextnode->nextptr;
			read_count++;
			loop_count--;
			}
		end = clock();
		cpu_time_in_ns = (1e9 * ( (double)(end - start)/CLOCKS_PER_SEC)) / read_count;
		fprintf(fp,"%lf,%d,%d,%d,%d,%lf\n", array_size_in_kb,thread_count,random_flag,seq_flag,write_flag,cpu_time_in_ns);
		//printf("For these settings: random_flag: %d, seq_flag: %d, write_flag: %d, size:%d, cputime:%f\n", random_flag, seq_flag, write_flag, array_size,cpu_time_in_ns);
		//printf("CPU time in ns is %f \n", cpu_time_in_ns);
		}

	else {
		//read-modify-write case
		clock_t start_rmw, end_rmw;
		double cpu_time_rmw;
		int rmw_count = 0;
		int loop_rmw_count = len;
		start_rmw = clock();
		struct Node* nextnode_rmw = node_list[1];
		uint64_t x;
		while (loop_rmw_count > 0) {
			x = nextnode_rmw->data;
			x = x ^ 0x7FFFFEEF;
			nextnode_rmw->data = x;
			nextnode_rmw = nextnode_rmw->nextptr;
			rmw_count++;
			loop_rmw_count--;
		}
		end_rmw = clock();
		cpu_time_rmw = (1e9 * ( (double)(end_rmw - start_rmw)/CLOCKS_PER_SEC)) / rmw_count;
		//fprintf(fp, "ArraySize,Thread_Count,Random,Sequential,Read_Modify_Write,CPU_TIME\n");
		fprintf(fp,"%lf,%d,%d,%d,%d,%lf\n", array_size_in_kb,thread_count,random_flag,seq_flag,write_flag,cpu_time_rmw);
		// random_flag, seq_flag, write_flag, array_size,cpu_time_rmw);

	}
	fclose(fp);

//end function
}
