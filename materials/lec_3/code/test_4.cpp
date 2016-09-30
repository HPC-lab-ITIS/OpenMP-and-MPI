#include <iostream>
#include <mpi.h>
    
int main(int argc, char *argv[])
{
    int rank; //process rank
    int size; //number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data = 0;

    std::cout << "Data in proccess " << rank << " = " << data << std::endl;
    
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0)
        data = 10;

    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Data in proccess " << rank << " = " << data << std::endl;

    MPI_Finalize();

    return 0;
}
