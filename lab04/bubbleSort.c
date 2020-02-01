#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps;
int printLevel;
int N, P;
int* v;
int *vQSort;
int *vNew;
pthread_barrier_t barrier;

int min(int x, int y)
{
	return (x < y) ? x : y;
}

void compareVectors(int * a, int * b) {
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
		if(!((i+1) % 20))
			printf("\n");
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

void getArgs(int argc, char **argv)
{
	if(argc < 4) {
		printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	NReps = atoi(argv[2]);
	printLevel = atoi(argv[3]);
 	P = atoi(argv[4]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	if(v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;
}

void printPartial()
{
	int i;
	compareVectors(v, vQSort);
}

void printAll()
{
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v, vQSort);
}

void print()
{
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printPartial();
	else
		printAll();
}

int par = 0;

void* bubble_sort(void* var)
{
	int thread_id = *(int*)var;
	int size = ceil(N / P);
	int start = size * thread_id;
	int end = min(size * (thread_id + 1), N - 1);
	int sorted = 0, i = par, aux;

	while (!sorted)
	{
		sorted = 1;
		for(i += start; i < end; i+=2)
		{
			if(v[i] > v[i + 1])
			{
				aux = v[i];
				v[i] = v[i + 1];
				v[i + 1] = aux;
				sorted = 0;
			}
		}
	}

	pthread_barrier_wait(&barrier);
	return NULL;
}

int main(int argc, char *argv[])
{
	int i, j, aux;
	getArgs(argc, argv);
	init();
	pthread_barrier_init(&barrier, NULL, P);

	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// sort the vector v
	// PARALLELIZE ME

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0; i < P; i++)
		thread_id[i] = i;

	for (j = 0; j < N; ++j)
	{
		//while (!sorted)
		///{

			for(i = 0; i < P; i++)
			{
				pthread_create(&(tid[i]), NULL, bubble_sort, &(thread_id[i]));
			}

			for(i = 0; i < P; i++)
				pthread_join(tid[i], NULL);

			par = (par + 1) % 2;
		//}
	}
	print();
	pthread_barrier_destroy(&barrier);
	return 0;
}
