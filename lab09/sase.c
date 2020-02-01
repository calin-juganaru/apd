#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

MPI_Status status;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number = world_rank;

    if (world_rank != world_size - 1)
    {
        MPI_Send(&number, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("Fara numar, fara numar, %d, thread %d.\n", number, status.MPI_SOURCE);
        }
    }

    MPI_Finalize();
}