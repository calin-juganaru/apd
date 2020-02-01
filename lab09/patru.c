#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int values[100];
    int recv[25];

    if (world_rank == 0)
    {
        for (int i = 0; i < 100; ++i)
            values[i] = i;
    }

    MPI_Scatter(values, 25, MPI_INT, recv, 25, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 25; ++i)
        recv[i] += 42;

    MPI_Gather(recv, 25, MPI_INT, values, 25, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d final:\n", world_rank);
    for (int i = 0; i < 100; ++i)
        printf("%d ", values[i]);
    printf("\n\n");

    MPI_Finalize();
}