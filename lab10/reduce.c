#include<mpi.h>
#include<stdio.h>
#include<math.h>

#define N 16

int main(int argc, char * argv[])
{
    int thread_id, P;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    int v[N] = {-9, 2, 2, -3, 5, 8, 2, -7, 4, -10, 1, 2, 3, 8, -1, 1};
    int sum = v[thread_id];

    for (int k = 2; k <= P; k *= 2)
    {
        if (thread_id % k  == 0)
        {
            int aux;
            MPI_Recv(&aux, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += aux;
        }

        if (thread_id % k == k / 2)
        {
            MPI_Send(&sum, 1, MPI_INT, thread_id - k / 2, 1, MPI_COMM_WORLD);
        }
    }

    if (thread_id == 0)
        printf("sum = %i\n", sum);

    MPI_Finalize();
    return 0;
}