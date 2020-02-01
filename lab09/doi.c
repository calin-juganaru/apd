#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int value = 0;

    if (world_rank == 0)
        value = 123456789;

    printf("Process %d initial:\n", world_rank);
    printf("%d \n\n", value);

    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d final:\n", world_rank);
    printf("%d \n\n", value);

    MPI_Finalize();
}