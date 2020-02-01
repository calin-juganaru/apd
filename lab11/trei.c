#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1000

int main(int argc, char * argv[])
{
	int rank, nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

	if (rank == 0)
    {
        for (int i = 0; i < N; ++i)
        {
            MPI_Send(&i, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        }
        printf("Got here! MPI_Send is non-blocking because there is no one to do a receive.\n");
	}
    else
    {
        for (int i = 0; i < 100; ++i)
        {
            MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Got here. Bafta coaie!\n");
        }
    }

	printf("\nBye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}