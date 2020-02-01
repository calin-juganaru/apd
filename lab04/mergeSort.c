#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

int NReps;
int printLevel;
int N, P;
int* v;
int* vQSort;

typedef struct
{
    int id, N;
    int* v;
} args_t;

void merge(int* v, int start, int end)
{
    int mid = (start + end) / 2;

    int* left = malloc((mid - start + 1) * sizeof(int));
    int* right = malloc((end - mid) * sizeof(int));

    int n1 = mid - start + 1,
		n2 = end - mid, i, j;

    for (i = 0; i < n1; i++)
        left[i] = v[i + start];

    for (i = 0; i < n2; i++)
        right[i] = v[i + mid + 1];

    int k = start;
    i = j = 0;

    while (i < n1 && j < n2)
	{
        if (left[i] <= right[j])
            v[k++] = left[i++];
        else
            v[k++] = right[j++];
    }

    while (i < n1)
        v[k++] = left[i++];

    while (j < n2)
        v[k++] = right[j++];
}

void seq_merge_sort(int* v, int begin, int end)
{
    int mid = begin + (end - begin) / 2;
    if (begin < end)
	{
        seq_merge_sort(v, begin, mid);
        seq_merge_sort(v, mid + 1, end);
        merge(v, begin, end);
    }
}

void* merge_sort(void* _args)
{
	args_t args = *(args_t*)_args;

	int size = args.N / P;
	int begin = size * args.id;
	int end = begin + size - 1;

	printf("Thread: %i | begin: %i | end: %i\n", args.id, begin, end);

	int mid = (begin + end) / 2;
    if (begin < end)
	{
        seq_merge_sort(v, begin, mid);
        seq_merge_sort(v, mid + 1, end);
        merge(v, begin, end);
    }
}

void compareVectors(int* a, int* b)
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

void displayVector(int* v)
{
	int displayWidth = 5;

	for(int i = 0; i < N; i++)
	{
		printf("%*i", displayWidth, v[i]);
		if(!((i+1) % 20))
			printf("\n");
	}
	printf("\n");
}

int cmp(const void *a, const void *b)
{
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

void getArgs(int argc, char** argv)
{
	if(argc < 4)
	{
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

	if(v == NULL)
	{
		printf("malloc failed!");
		exit(1);
	}

	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;
}

void printPartial()
{
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

int main(int argc, char *argv[])
{
	int i, j;
	getArgs(argc, argv);
	init();

	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	pthread_t tid[P];
	args_t args[P];

    for (int i = 0; i < P; ++i)
	{
		args[i].N = N;
		args[i].v = v;
		args[i].id = i;
	}

	for (int i = 0; i < P; ++i)
	{
        pthread_create(&(tid[i]), NULL, merge_sort, &args[i]);
	}

	for (int i = 0; i < P; i++)
	{
		pthread_join(tid[i], NULL);
	}

	for (int length = N / P; length <= N; length *= 2)
		for (int i = 0; i < N - length; i += 2 * length)
			merge(v, i, i + 2 * length - 1);

	print();

	return 0;
}
