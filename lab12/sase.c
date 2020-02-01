#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int thread_id, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int* values = malloc(sizeof(int) * N);
    int* recv = malloc(sizeof(int) * N / P);
    clock_t begin, end;

    if (thread_id == 0)
    {
        begin = clock();
        for (int i = 0; i < N; ++i)
            values[i] = i;
    }

    MPI_Scatter(values, N / P, MPI_INT, recv, N / P, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N / P; ++i)
        recv[i] += 42;

    MPI_Gather(recv, N / P, MPI_INT, values, N / P, MPI_INT, 0, MPI_COMM_WORLD);

    if (thread_id == 0)
    {
        end = clock();
	    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
    }

    MPI_Finalize();
}