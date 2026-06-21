#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "papi.h"

#define N 1000
#define NUM_THREADS 48

int main(void) 
{
	int i = 0;
	int x[N];
	int y[N];

	int retval;
	if ((retval = PAPI_hl_region_begin("false_sharing")) != PAPI_OK) {
    		fprintf(stderr, "PAPI_hl_region_begin error! %d, %d\n", retval, __LINE__);
    		exit(1);
  	}

	double sum  = 0.0;
	double sum_local[NUM_THREADS];

	#pragma omp parallel num_threads(NUM_THREADS)
	{
		int iam = omp_get_thread_num();
		#pragma omp for
		for (i = 0; i< N; i++) {
		       sum_local[iam] = x[i] * y[i];
		}

		#pragma omp atomic
		sum += sum_local[iam];
	
	}	

	if ((retval = PAPI_hl_region_end("false_sharing")) != PAPI_OK) {
    	fprintf(stderr, "PAPI_hl_region_end error! %d, %d\n", retval, __LINE__);
    	exit(1);
  	}

	return 0;
}
