#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double compute_pi()
{
    unsigned long circle_points = 0, square_points = 0;

    for (unsigned long i = 1000000000; i; --i)
    {
        double x = 1.0 * rand() / (RAND_MAX + 1.0);
        double y = 1.0 * rand() / (RAND_MAX + 1.0);

        if (x*x + y*y <= 1.0)
            circle_points += 4;
        square_points += 4;
    }

    return 4.0 * circle_points / square_points;
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    srand(time(NULL));
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank != world_size - 1)
    {
        double PI = compute_pi();
        MPI_Send(&PI, 1, MPI_DOUBLE, world_size - 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        double PI = compute_pi();
        for (int i = 0; i < world_size - 1; ++i)
        {
            double number;
            MPI_Recv(&number, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            PI += number;
        }
        printf("PI e cam atata %lf.\n", PI / world_size);
    }

    MPI_Finalize();
}