#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 12
#define M 30

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

int neighbour_count;

int* getNeighbours(int node)
{
 	int *neighbours, n = 0; neighbour_count = 0;

 	for (int i = 0; i < M; ++i)
 		if (graph[i][0] == node) ++n;
    neighbours = calloc(n, sizeof(int));

 	for (int i = 0; i < M; ++i)
 		if (graph[i][0] == node)
 			neighbours[neighbour_count++] = graph[i][1];

 	return neighbours;
}


void epidemic(int thread_id)
{
	int* neighbours = getNeighbours(thread_id);
	float x = (thread_id == N - 1) ? 1 : 0, y;

	for (int i = 0; i < N * N * N; ++i)
		for (int j = 0; j < neighbour_count; j ++)
        {
			MPI_Sendrecv(&x, 1, MPI_FLOAT, neighbours[j], 1, &y, 1, MPI_FLOAT,
						 MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			x = (x + y) / 2;
		}

	printf("Node %i says there are %f nodes\n", thread_id, 1.0f / x);
}


int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int thread_id, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

	printf("Hello from %i/%i\n", thread_id, P);
	epidemic(thread_id);
	printf("Bye from %i/%i\n", thread_id, P);

	MPI_Finalize();
	return 0;
}