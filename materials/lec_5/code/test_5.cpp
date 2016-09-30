#include <iostream>
#include <vector>
#include <cmath>
#include "profiler.h"
#include <algorithm>
#include <mpi.h>
#include <random>

int main (int argc, char ** argv) 
{
    const long n = 1024*1024*256;
    profiler prof;
    int rank, size;
    double runtime = -MPI_Wtime();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);

    std::vector<int> unsort(n);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 100);

    if(rank == 0)
    {
        for(auto i = 0; i < n; ++i)
            unsort[i] = uni(rng);
    }

    long int chunk = n / size;
    std::vector<int> local(chunk);

    MPI_Scatter(unsort.data(), chunk, MPI_INT, local.data(), chunk, MPI_INT, 0, MPI_COMM_WORLD);

    std::sort(local.begin(), local.end());

    long int step_1 = 1, step_2 = 2, buf_size = 0;
    std::vector<int> recv_buf;
    std::vector<int> tmp;

    while(step_1 < size)
    {
        for(long int i = 0; i < size - step_1; i += step_2)
        {
            if(rank == i + step_1)
            {
                buf_size = (long int)local.size();
                MPI_Send(&buf_size, 1, MPI_LONG, i, 0, MPI_COMM_WORLD); //size of buffer
            }

            if(rank == i)
            {
                MPI_Recv(&buf_size, 1, MPI_LONG, i + step_1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//size of buffer
                recv_buf.resize(buf_size);
            }

            if(rank == i + step_1)
                MPI_Send(local.data(), buf_size, MPI_INT, i, 0,  MPI_COMM_WORLD); //buffer

            if(rank == i)
            {
                MPI_Recv(recv_buf.data(), buf_size, MPI_INT, i + step_1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//buffer
                tmp.resize(local.size() + buf_size);
                std::merge(local.begin(), local.end(), recv_buf.begin(), recv_buf.end(), tmp.data());
                local = tmp;
            }
        }
        step_1 = step_2;
        step_2 *= 2;
    }

    runtime += MPI_Wtime();

    if(rank == 0)
    {
        std::cout << runtime << std::endl;
        prof.tic("seq");
        std::sort(unsort.begin(), unsort.end());
        prof.toc("seq");
        prof.report();
    }
    
    MPI_Finalize();

    return 0;
}
