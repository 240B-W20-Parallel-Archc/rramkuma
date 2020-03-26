/*
 * cacheperf.h
 *
 *  Created on: Mar 12, 2020
 *      Author: ranga.ramkumar
 */

#ifndef CACHEPERF_H_
#define CACHEPERF_H_

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>


//Functions that read input arguments and decides thread/seq etc
// Run ./cacheperf --options

//Defaults
void performance_latency();

#endif /* CACHEPERF_H_ */
