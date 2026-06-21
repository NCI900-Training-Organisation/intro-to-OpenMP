#include <iostream>
#include <random>

int main() {

    const long N = 100000000;

    long inside = 0;

    #pragma omp target teams distribute parallel for reduction(+:inside)
    for(long i=0;i<N;i++) {

        unsigned int seed = i;

        float x =
            (float)rand_r(&seed)/(float)RAND_MAX;

        float y =
            (float)rand_r(&seed)/(float)RAND_MAX;

        if(x*x + y*y <= 1.0f)
            inside++;
    }

    double pi =
        4.0 * (double)inside / (double)N;

    std::cout << "Pi = " << pi << "\n";
}