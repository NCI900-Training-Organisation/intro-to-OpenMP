#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "papi.h"

#define N 100
#define NUM_THREADS 10
#define CACHE_LINE_SIZE 16

int main(void) 
{
	int i = 0;
	int x[N];
	int y[N];

	int retval;
	if ((retval = PAPI_hl_region_begin("no_false_sharing")) != PAPI_OK) {
    		fprintf(stderr, "PAPI_hl_region_begin error! %d, %d\n", retval, __LINE__);
    		exit(1);
  	}

	double sum  = 0.0;
	double sum_local[NUM_THREADS][CACHE_LINE_SIZE];

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int iam = omp_get_thread_num();
		#pragma omp for
		for (i = 0; i< N; i++) {
		       sum_local[iam][0] = x[i] * y[i];
		}

		#pragma omp atomic
		sum += sum_local[iam][0];
	
	}	

	if ((retval = PAPI_hl_region_end("no_false_sharing")) != PAPI_OK) {
    	fprintf(stderr, "PAPI_hl_region_end error! %d, %d\n", retval, __LINE__);
    	exit(1);
  	}

	return 0;
}
