#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N, P;
int** v;
int** u;
pthread_barrier_t barrier;

int min(int x, int y)
{
	return (x < y) ? x : y;
}

void displayVector(int** v)
{
	int i, j;
	int displayWidth = 2;
	for(j = 0; j < N; j++)
    {
        if (j % 2)
            for(i = 0; i < N; i++)
            {
                printf("%*i", displayWidth, v[j][i]);
                if(!((i+1) % 20))
                    printf("\n");
            }
        else
            for(i = N - 1; i >= 0; i--)
            {
                printf("%*i", displayWidth, v[j][i]);
                if(!((i+1) % 20))
                    printf("\n");
            }

    }
    printf("\n");
}

int cmp1(const void *a, const void *b)
{
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int cmp2(const void *a, const void *b)
{
	int A = *(int*)a;
	int B = *(int*)b;
	return B - A;
}

void init()
{
	int i, j;
	v = malloc(sizeof(int*) * N);
    u = malloc(sizeof(int*) * N);
	srand(42);
	for(i = 0; i < N; i++)
    {
        v[i] = malloc(sizeof(int) * N);
        u[i] = malloc(sizeof(int) * N);
        for(j = 0; j < N; j++)
		    v[i][j] = rand() % N;
    }
}

void print()
{
	displayVector(v);
}

void transpus()
{
    int i, j;
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
            u[i][j] = v[j][i];
    }
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
            v[i][j] = u[i][j];
    }
}

void* shear(void* arg)
{
    int thread_id = *(int*)arg;
    //printf("Hello world from thread %i\n", thread_id);
    int size = N / P;
	int start = size * thread_id;
	int end = start + size;
    //pthread_barrier_wait(&barrier);
    for (int lg = 0; lg <= log2(N); lg++)
    {
        for(int i = start; i < end; i++)
        {
            if (i % 2)
                 qsort(v[i], N, sizeof(int), cmp1);
            else qsort(v[i], N, sizeof(int), cmp2);
        }

        //pthread_barrier_wait(&barrier);

        for(int i = start; i < end; i++)
        {
            transpus();
            qsort(v[i + start], size, sizeof(int), cmp1);
            transpus();
        }

        //pthread_barrier_wait(&barrier);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
	int i, j, aux, lg;
	N = atoi(argv[1]);
    P = atoi(argv[2]);
	init();

    pthread_barrier_init(&barrier, NULL, P);

    pthread_t tid[P];
	int thread_id[P];

	for(i = 0; i < P; i++)
		thread_id[i] = i;

	for(i = 0; i < P; i++)
	{
		pthread_create(&(tid[i]), NULL, shear, &(thread_id[i]));
	}

	for(i = 0; i < P; i++)
	{
		pthread_join(tid[i], NULL);
	}

	print();
    pthread_barrier_destroy(&barrier);
	return 0;
}
