/*
 * main.c
 *
 *  Created on: Mar 23, 2020
 *      Author: ranga.ramkumar
 */
#include "cacheperf.h"


FILE *stream;
FILE *fp;
int thread_count = 1;
int array_size = 1024;
bool seq_flag = false;
bool random_flag = false;
bool write_flag = false;
int stride = 1;
int array_size_in_kb = 1;
//(int) (array_size / 1024);

void print_usage() {
	fprintf(stderr, "Usage: ./cacheperf <options> \n");
	fprintf(stderr, "Options: \n");
	fprintf(stderr, "	-t:<threads> \n");
	fprintf(stderr, "	-s/-r (seq or random access) \n");
	fprintf(stderr, "	-size:<array_size> (in KB) \n");
	fprintf(stderr, " 	-stride:<stride length> (only for use with sequential) \n");
}

int handling_inputs(char *arg) {
	if(!(strncmp(arg,"-t:",3))) {
		sscanf(arg+3,"%d", &thread_count);
	} else if (!strncmp(arg,"-size:",6)) {
		sscanf(arg+6, "%d", &array_size_in_kb);
		array_size = 1024*array_size_in_kb;
	} else if (!strncmp(arg,"-s",2)) {
		seq_flag = true;
	} else if (!strncmp(arg,"-r",2)) {
		random_flag = true;
	} else if (!strncmp(arg, "-write",6)) {
		write_flag = true;
	} else if (!strncmp(arg, "-stride:", 8)){
		sscanf(arg+8,"%d",&stride);
	} else {
		return 0;
	}
	return 1;
}


int main(int argc, char *argv[]) {
	//Setting defaults
	stream = stdin;

	for (int i = 1; i < argc; i++) {
		if(!(strcmp(argv[i],"--help"))) {
			print_usage();
			//return 0;
		} else if (!strncmp(argv[i],"-",1)){
			handling_inputs(argv[i]);
			if(!handling_inputs(argv[i])) {
				fprintf(stderr, "ERROR: Unrecognized options!\n");
				fprintf(stderr, "Refer to the following:\n");
				print_usage();
				exit(0);
			}
		} else {
			// forgot about the case when there is no - in input
			fprintf(stderr, "beepbop\n");
			fprintf(stderr, "ERROR: Unrecognized options!\n");
			fprintf(stderr, "Refer to the following:\n");
			print_usage();
			exit(0);
		}
	}

	//thread creation and joining to be done here
	/*
	 * int pthread_create(pthread_t * thread,
                   const pthread_attr_t * attr,
                   void * (*start_routine)(void *),
                   void *arg);

       int pthread_join(pthread_t th, void **thread_return);
	 */

	if(thread_count > 1) {
		pthread_t* threads = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
		for (int i = 0; i < thread_count; i++) {
			pthread_create(threads + i, NULL, performance_latency, NULL);
		}
		for (int i = 0; i < thread_count; i++) {
			pthread_join(*(threads + i), NULL);
		}
		free(threads);
	} else {
	// only one thread
	performance_latency();
	}
	return 0;
}
