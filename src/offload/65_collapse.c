#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    const int NX = 10000;
    const int NY = 10000;

    long inside = 0;

    double start = omp_get_wtime();

    #pragma omp target teams distribute parallel for \
            collapse(2) \
            num_teams(1024) \
            thread_limit(256) \
            reduction(+:inside)
    for (int i = 0; i < NX; i++)
    {
        for (int j = 0; j < NY; j++)
        {
            float x = (float)i / (float)NX;
            float y = (float)j / (float)NY;

            if (x * x + y * y <= 1.0f)
            {
                inside++;
            }
        }
    }

    double end = omp_get_wtime();

    printf("Grid points: %d x %d\n", NX, NY);
    printf("Inside = %ld\n", inside);

    printf("Execution Time = %.6f seconds\n",
           end - start);

    return 0;
}