#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    printf("Process %d:\n", world_rank);

    int v[100];
    if (world_rank == 0)
    {
        for (int i = 0; i < 100; ++i)
            v[i] = i;
    }

    MPI_Bcast(v, 100, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < 100; ++i)
        printf("%d ", v[i]);
    printf("\n\n");

    MPI_Finalize();
}