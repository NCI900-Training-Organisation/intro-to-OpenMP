#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "papi.h"

#define N 100000

/* This program demonstrates the PAPI high-level API (PAPI 6.0+).
   The events to record are selected at run time via the PAPI_EVENTS
   environment variable, and the report is printed to stdout when
   PAPI_REPORT=1, e.g.:

     export PAPI_EVENTS="PAPI_TOT_CYC,PAPI_L1_TCM,PAPI_LD_INS,PAPI_SR_INS"
     export PAPI_REPORT=1
     ./papi
*/

int main(void)
{
	int i = 0;
	double x[N];
	double y[N];

	int retval;
	if ((retval = PAPI_hl_region_begin("computation")) != PAPI_OK) {
		fprintf(stderr, "PAPI_hl_region_begin error! %d, %d\n", retval, __LINE__);
		exit(1);
	}

	double sum  = 0.0;

	for (i = 0; i< N; i++) {
	       sum = x[i] * y[i];
	}

	if ((retval = PAPI_hl_region_end("computation")) != PAPI_OK) {
		fprintf(stderr, "PAPI_hl_region_end error! %d, %d\n", retval, __LINE__);
		exit(1);
	}

	return 0;
}
