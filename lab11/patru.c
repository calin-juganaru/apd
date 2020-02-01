#include <mpi.h>
#include <stdio.h>

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int graph[][2] =
{
    { 0, 1 }, { 1, 2 }, { 2, 3 },
    { 3, 4 }, { 4, 11 }, { 11, 5 },
    { 5, 6 }, { 6, 7 }, { 7, 8 },
    { 8, 9 }, { 9, 10 }, { 10, 9 },
    { 9, 8 }, { 8, 7 }, { 7, 6 },
    { 6, 5 }, { 5, 11 }, { 11, 4 },
    { 4, 3 }, { 3, 2 }, { 2, 1 },
    { 1, 0 }, { 9, 5 }, { 5, 9 },
    { 5, 3 }, { 3, 5 }, { 0, 2 },
    { 2, 0 }, { 9, 7 }, { 7, 9 }
};

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Request request;

    int thread_id, nProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    int max = thread_id, aux;

    for (int i = 0; i < nProcesses; ++i)
        for (int j = 0; j < 30; ++j)
            if (graph[j][0] == thread_id)
            {
                MPI_Send(&max, 1, MPI_INT, graph[j][1], 0, MPI_COMM_WORLD);
                MPI_Recv(&aux, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                max = MAX(max, aux);
            }

    printf("%d - %d\n", thread_id, max);
    MPI_Finalize();
    return 0;
}