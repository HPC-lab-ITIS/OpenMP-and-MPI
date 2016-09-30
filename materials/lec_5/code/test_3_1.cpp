#include <iostream>
#include <vector>
#include "profiler.h"
#include "mpi.h"
#include "boost/multi_array.hpp"
#include "boost/range/algorithm.hpp"
#include <random>
#include <algorithm>

int main (int argc, char ** argv) 
{
    long long root, offset;
    int rank,size;
    double runtime;

    typedef boost::multi_array<int, 2> matrix;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);
   
    int n = 1024;
    int inf = 1E6;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 10);

    matrix a(boost::extents[n][n]);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == j)
                a[i][j] = 0;
            else
            {
                a[i][j] = (int)uni(rng);
    
                if(a[i][j] == 10)
                    a[i][j] = inf;
            }

    runtime = -MPI_Wtime();
    long long low_value  = 2 + (long long)rank * n / (long long)size;
    long long high_value = 1 + (long long)(rank + 1) * n / (long long)size;
    long long chunk = high_value - low_value + 1;
    std::vector<int> tmp(n);
    for(int k = 0; k < n; ++k)
    {
        root = (size*(k + 1) - 1)/n;
        if(root == rank)
        {
            offset = k*low_value;
            for(int i = 0; i < n ; ++i)
                tmp[i] = a[offset][i];
        }

        MPI_Bcast(tmp.data(), n, MPI_INT, root, MPI_COMM_WORLD);
        
        for(int i = 0; i < chunk; ++i)
            for(int j = 0; j < n; ++j)
                a[i][j] = std::min(a[i][j],a[i][k] + tmp[j]);
    }
    runtime += MPI_Wtime();

    if (rank == 0)
        std::cout << "time = " << runtime << " sec." << std::endl;

    MPI_Finalize();
    return 0;
}
