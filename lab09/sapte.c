#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number = world_rank;
    MPI_Status status;

    if (world_rank == 0)
    {
        MPI_Send(&number, 1, MPI_INT, world_size - 1, 123456789, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&number, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Fara numar, fara numar, %d, tag %d.\n", number, status.MPI_TAG);
    }

    MPI_Finalize();
}