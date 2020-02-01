#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int graph[][2] =
{
	{ 0, 1 }, { 0, 4 }, { 0, 5 },
    { 1, 0 }, { 1, 2 }, { 1, 6 },
    { 2, 1 }, { 2, 3 }, { 2, 7 },
    { 3, 2 }, { 3, 4 }, { 3, 8 },
    { 4, 0 }, { 4, 3 }, { 4, 9 },
    { 5, 0 }, { 5, 7 }, { 5, 8 },
    { 6, 1 }, { 6, 8 }, { 6, 9 },
    { 7, 2 }, { 7, 5 }, { 7, 9 },
    { 8, 3 }, { 8, 5 }, { 8, 6 },
    { 9, 4 }, { 9, 6 }, { 9, 7 }
};

int neighbour_count = 0, N = 10;

int* getNeighbours(int node)
{
 	int *neighbours, n = 0;
 	neighbour_count = 0;
 	for (int i = 0; i < N; i ++)
 		if (graph[i][0] == node) ++n;

 	neighbours = malloc(n * sizeof(int));
 	for (int i = 0; i < N; ++i)
 		if (graph[i][0] == node)
 			neighbours[neighbour_count++] = graph[i][1];

 	return neighbours;
}

int* getBFS(int root)
{
	int *visited = malloc(N * sizeof(int));
	int *queue = malloc(N * N * sizeof(int));
	int start = 0, finish;

	for (int i = 0; i < N; i ++)
		visited[i] = -1;

	int *neigh = getNeighbours(root);
	for (int i = 0; i < neighbour_count; i ++)
	{
		visited[neigh[i]] = neigh[i];
		queue[i] = neigh[i];
	}

	visited[root] = root;
	finish = neighbour_count;

	while (start <= finish)
	{
		int *neigh = getNeighbours(queue[start ++]);
		for (int i = 0; i < neighbour_count; ++i)
			if (visited[neigh[i]] == -1)
			{
				visited[neigh[i]] = visited[queue[start - 1]];
				queue[finish++] = neigh[i];
			}
	}
	return visited;
}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int thread_id, P;
	MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	printf("Hello from %i/%i\n", thread_id, P);

	int *neighbours = getNeighbours(thread_id);
	for (int i = 0; i < neighbour_count; i ++)
		printf("%i is neigh with %i\n", thread_id, neighbours[i]);

	neighbours = getBFS(thread_id);
	for (int i = 0; i < N; i ++)
		printf("Fastest route from %i to %i starts in %i\n", thread_id, i, neighbours[i]);

	printf("Bye from %i/%i\n", thread_id, P);
	MPI_Finalize();
	return 0;
}