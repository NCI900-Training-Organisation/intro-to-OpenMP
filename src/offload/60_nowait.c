#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    const long N = 100000000;

    long inside = 0;

    double start = omp_get_wtime();

    #pragma omp target data map(tofrom: inside)
    {

        #pragma omp target teams distribute parallel for \
                num_teams(1024) \
                thread_limit(256) \
                reduction(+:inside) \
                nowait
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

        // -----------------------------------------
        // CPU does independent work while GPU runs
        // -----------------------------------------
        long cpu_work = 0;
        for (long i = 0; i < 50000000; i++)
        {
            cpu_work += i % 3;
        }

        printf("CPU work done: %ld\n", cpu_work);
    }

    double end = omp_get_wtime();

    double pi =
        4.0 * (double)inside / (double)N;

    printf("Inside = %ld\n", inside);
    printf("Pi ~= %.10f\n", pi);
    printf("Execution Time = %.6f seconds\n", end - start);

    return 0;
}