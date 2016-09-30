#include <iostream>
#include <mpi.h>
    
int main(int argc, char *argv[])
{
    int rank; //process rank
    int size; //number of processes

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int number = rank + 1;
    
    if ( rank < (size - 1) )
    {
        MPI_Send(&number, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        if(!rank)
            MPI_Recv(&number, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        else
            MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received number " << number << " from process " << (rank ? rank - 1 : size - 1) << std::endl;
    }
    else
    {
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&number, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received number " << number << " from process " << rank - 1 << std::endl;
    }
    
    MPI_Finalize();

    return 0;
}
