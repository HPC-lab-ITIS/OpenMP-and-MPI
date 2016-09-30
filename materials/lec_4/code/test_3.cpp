#include <iostream>
#include <mpi.h>
    
int main(int argc, char *argv[])
{
    int rank; //process rank
    int size; //number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 1)
    { 
        MPI_Barrier(MPI_COMM_WORLD);
        std::cout << "Hello" << std::endl;
    }
    else
        std::cout << "Hi" << std::endl;

    MPI_Finalize();

    return 0;
}
