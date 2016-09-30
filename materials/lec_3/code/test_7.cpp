#include <iostream>
#include <vector>
#include "profiler.h"
#include <random>
#include <algorithm>
#include <mpi.h>
#include <numeric>

int main (int argc, char ** argv) 
{
    int rank; //process rank
    int size; //number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 256*1024*1024;
    int loc_num = n / size;
    
    std::vector<double> elems;
    double test_result;   
    double runtime = -MPI_Wtime();

    if(rank == 0)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<double> uni(1., 10.);

        for(int i = 0; i < n; ++i)
            elems.push_back( (double)uni(rng) );

        test_result = std::accumulate(elems.begin(), elems.end(), 0.) / ((double)n);
    }

    std::vector<double> loc_elems(loc_num);

    MPI_Scatter(elems.data(), loc_num, MPI_DOUBLE, loc_elems.data(), loc_num, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double loc_avg = std::accumulate(loc_elems.begin(), loc_elems.end(), 0.) / ((double)loc_num);

    double result = 0.;

    MPI_Reduce(&loc_avg, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    result /= (double)size;
    
    runtime += MPI_Wtime();
    
    if(rank == 0)
    {    
        std::cout <<  ( (fabs(result - test_result) < 0.000001 ) ? "Correct result" : "Invalid result")   << std::endl;
        std::cout << "Time = " << runtime << std::endl;
    }

    MPI_Finalize();
    
    return 0;
}
