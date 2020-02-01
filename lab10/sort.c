#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 16

void compareVectors(int * a, int * b)
{
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
	}
	printf("\n");
}

int cmp(const void *a, const void *b)
{
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int main(int argc, char* argv[])
{
	int thread_id, P;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

	int val = -1;

	if (thread_id != 0)
	{
		for (int i = 1; i <= P - thread_id; i++)
		{
			int aux;
			MPI_Recv(&aux, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			if (val == -1)
			{
				val = aux;
			}
			else
			{
				if (aux < val)
				{
					MPI_Send(&val, 1, MPI_INT, thread_id + 1, 0, MPI_COMM_WORLD);
					val = aux;
				}
				else
				{
					MPI_Send(&aux, 1, MPI_INT, thread_id + 1, 0, MPI_COMM_WORLD);
				}
			}
		}
	}
	else
	{
		int v[N];
		srand(42);

		for(int i = 0; i < N; i++)
		{
			v[i] = rand() % N;
		}

		for (int i = 0; i < N; i++)
		{
			MPI_Send(&v[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		}

	}

	if (thread_id != 0)
	{
		printf("thread_id is %d, value is %d\n", thread_id, val);
	}

	MPI_Finalize();
	return 0;
}