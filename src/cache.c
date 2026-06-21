#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "papi.h"

#define CACHE 819600

int main(int argc, char* argv[]) 
{

	if (argc != 2) {
        printf("Pass a data size \n");
        return -1;
    } 

	int dataSize = atoi(argv[1]);

	int cacheLineSize = 64;
	int cacheSize = 32768;
	int nbCacheline = cacheSize / cacheLineSize;
	int c =10;
	int arr[dataSize];

	int retval;
	if ((retval = PAPI_hl_region_begin("cache_access")) != PAPI_OK) {
    		fprintf(stderr, "PAPI_hl_region_begin error! %d, %d\n", retval, __LINE__);
    		exit(1);
  	}

	int i = 0;
	for (i = 0; i < dataSize; i++) {
	       arr[i] = arr[i] + 10;
	}

	if ((retval = PAPI_hl_region_end("cache_access")) != PAPI_OK) {
    	fprintf(stderr, "PAPI_hl_region_end error! %d, %d\n", retval, __LINE__);
    	exit(1);
  	}

	return 0;
}
