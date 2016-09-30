#include <iostream>
#include <vector>
#include "profiler.h"
#include "mpi.h"
#include <math.h>

int main (int argc, char ** argv) 
{
    int index;
    int prime;
    int count;
    int global_count;
    int first;
    long long high_value, low_value;
    int rank,size;
    double runtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    
    runtime = -MPI_Wtime();

    long long n = 1E9;

    low_value  = 2 + (long long)rank * n / (long long)size;
    high_value = 1 + (long long)(rank + 1) * n / (long long)size;
    long long chunk = high_value - low_value + 1;

    std::vector<bool> marked(chunk, false);

    if(rank == 0) 
        index = 0;
    prime = 2;

    do 
    {
        if (prime * prime > low_value) 
            first = prime * prime - low_value;
        else 
        {
            if ((low_value % prime) == 0) 
                first = 0;
            else 
                first = prime - (low_value % prime);
        }

        for(int j = first; j < marked.size(); j += prime) 
            marked[j] = true;

        if(rank == 0) 
        {
            while (marked[++index]);
            prime = index + 2;
        }

        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }
    while( prime*prime <= n );

    count = 0;

    for(int i = 0; i < marked.size(); i++) 
        if(!marked[i])
            count++;

    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    runtime += MPI_Wtime();

    if (rank == 0)
        std::cout << "time = " << runtime << " sec." <<std::endl << global_count << std::endl;

    MPI_Finalize();
    return 0;
}
