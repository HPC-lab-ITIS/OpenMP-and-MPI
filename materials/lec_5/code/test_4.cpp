#include <iostream>
#include <vector>
#include "profiler.h"
#include "boost/multi_array.hpp"
#include <random>
#include <algorithm>
#include <mpi.h>

int main (int argc, char ** argv) 
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);

    typedef boost::multi_array<double, 2> matrix;

    profiler prof;
    int n = 1024;
    int inf = 1E6;

    int chunk = n / size;

    std::random_device rd;
    std::mt19937 rng_1(rd());
    std::mt19937 rng_2(rd());
    std::uniform_real_distribution<double> uni(0.1, 10.0);

    matrix a(boost::extents[n][n]);
    matrix b(boost::extents[n][n]);
    matrix c(boost::extents[n][n]);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            c[i][j] = 0.;

    if(rank == 0)
    {
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j)
            {
                a[i][j] = uni(rng_1);
                b[i][j] = uni(rng_2);
            }
    }

    double runtime = -MPI_Wtime();
    
    MPI_Bcast(a.data(), n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.data(), n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(int i = rank*chunk; i < (rank + 1)*chunk - 1; ++i)
        for(int j = 0; j < n; ++j)
            for(int k = 0; k < n; ++k)
                c[i][j] += a[i][k]*b[k][j];

    MPI_Gather(c.data() + chunk*rank, n*chunk, MPI_DOUBLE, c.data() + chunk*rank, n*chunk,  MPI_DOUBLE, 0, MPI_COMM_WORLD);

    runtime += MPI_Wtime();

    if(rank == 0)
        std::cout << runtime << std::endl;

    MPI_Finalize();

    return 0;
}
