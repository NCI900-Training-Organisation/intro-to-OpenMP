#include <stdio.h>
#include <omp.h>

int main(void)
{
    #pragma omp target
    {
        for (int i = 0; i < 10; i++)
        {
            printf("%d\n", i);
        }
    }

    return 0;
}