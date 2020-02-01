#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number = 0;

    if (world_rank == 0)
    {
        MPI_Send(&number, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
        printf("Fara numar, fara numar, %d.\n", number);
        MPI_Recv(&number, 1, MPI_INT, (world_rank - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Recv(&number, 1, MPI_INT, (world_rank - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        number += 2;
        printf("Fara numar, fara numar, %d.\n", number);
        MPI_Send(&number, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}