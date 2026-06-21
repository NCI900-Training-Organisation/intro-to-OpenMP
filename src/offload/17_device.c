#include <stdio.h>
#include <omp.h>

int main(void)
{
    printf("Host: device %d\n", omp_get_initial_device());

    #pragma omp target
    {
        printf("Inside target region: device %d\n",
               omp_get_device_num());
    }

    return 0;
}