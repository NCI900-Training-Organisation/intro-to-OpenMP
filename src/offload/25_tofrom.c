#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    const long N = 100000000;

    long inside = 0;

    double start = omp_get_wtime();

    #pragma omp target map(tofrom:inside)
    {
        for (long i = 0; i < N; i++)
        {
            unsigned int seed = (unsigned int)i;

            float x =
                (float)rand_r(&seed) /
                (float)RAND_MAX;

            float y =
                (float)rand_r(&seed) /
                (float)RAND_MAX;

            if (x * x + y * y <= 1.0f)
            {
                inside++;
            }
        }
    }

    double end = omp_get_wtime();

    printf("Pi ~= %.10f\n",
           4.0 * (double)inside / (double)N);

    printf("Execution Time = %.6f s\n",
           end - start);

    return 0;
}