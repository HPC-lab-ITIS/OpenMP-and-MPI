#include <iostream>
#include <vector>
#include <mpi.h>
#include <random>
#include <algorithm>

int main (int argc, char ** argv) 
{
    long int n = 1E10;

    int rank,size;
    double runtime = -MPI_Wtime();


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long int chunk = n / size;
    
    std::random_device rd_1;
    std::random_device rd_2;
    std::mt19937 rng_x(rd_1());
    std::mt19937 rng_y(rd_2());
    std::uniform_real_distribution<double> dist(0., 1.);

    double x, y;
    long int count = 0;

    for(auto i = rank*chunk; (i < rank*chunk + chunk) && (i < n); ++i)
    {
        x = (double)dist(rng_x);
        y = (double)dist(rng_y);

        if(x*x + y*y <= 1.)
            ++count;
    }

    long int global_count = 0;

    MPI_Reduce(&count, &global_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    runtime += MPI_Wtime();
    
    if(rank == 0)
    {
        double pi = 4. * (double)global_count / (double)n;

        std::cout << pi << std::endl;
        std::cout << runtime << std::endl;
    }

    MPI_Finalize();
    return 0;
}
