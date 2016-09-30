#include <iostream>
#include <mpi.h>
    
int main(int argc, char *argv[])
{
    int rank; //process rank
    int size; //number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "Hello, world from process number " << rank << " of " << size << std::endl;
     
    MPI_Finalize();

    return 0;
}
