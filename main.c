/*
 * main.c
 *
 *  Created on: Mar 23, 2020
 *      Author: ranga.ramkumar
 */
#include "cacheperf.h"

FILE *stream;
int thread_count = 1;
int array_size = 1024;
bool seq_flag = false;
bool random_flag = false;
bool write_flag = false;

void print_usage() {
	fprintf(stderr, "Usage: ./cacheperf <options> \n");
	fprintf(stderr, "Options: \n");
	fprintf(stderr, "	-t:<threads> \n");
	fprintf(stderr, "	-s/-r (seq or random access) \n");
	fprintf(stderr, "	-size:<array_size> (in KB) \n");
}

int handling_inputs(char *arg) {
	if(!(strncmp(arg,"-t:",3))) {
		sscanf(arg+3,"%d", &thread_count);
	} else if (!strncmp(arg,"-size:",6)) {
		int array_size_input;
		sscanf(arg+6, "%d", &array_size_input);
		array_size = 1024*array_size_input;
	} else if (!strncmp(arg,"-s",2)) {
		seq_flag = true;
	} else if (!strncmp(arg,"-r",2)) {
		random_flag = true;
	} else if (!strncmp(arg, "-write",6)) {
		write_flag = true;
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
			// fixing case when there is no - in input
			fprintf(stderr, "beepbopm");
			fprintf(stderr, "ERROR: Unrecognized options!\n");
			fprintf(stderr, "Refer to the following:\n");
			print_usage();
			exit(0);
		}
	}
	//one function for each of four cases?
	//fold these inside if-else or switch loops
	performance_rs();
	performance_rws();
	performance_rm();
	performance_rwm();
	return 0;
}
