#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Request request;
    MPI_Status status;

    int thread_id, P, number;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (thread_id == 0)
    {
        number = 123456789;
        MPI_Isend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        number = -123456789;
        MPI_Wait(&request, &status);
    }
    else
    {
        MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        printf("Fara numar, fara numar, %d...\n", number);
    }

    MPI_Finalize();
}