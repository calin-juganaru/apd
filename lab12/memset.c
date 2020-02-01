#include<string.h>
#include<stdio.h>
#include<time.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

#define MIN(x, y) ((x < y) ? x : y)

int n1GB = 1024*1024*1024;
char v[1024*1024*1024];
int P;

void* sqrt_f(void *var)
{
	int N = 32, aux = 32*1024*1024;
	int start, stop, thread_id = *(int*)var;
	start = thread_id * ceil(N / P);
	stop = MIN((thread_id + 1) * ceil(N / P), N);
	for (int i  = start; i < stop; ++i)
		memset(v + aux * i, '1', aux);
}

int main(int argc, char * argv[])
{
	P = atoi(argv[1]);

	pthread_t tid[P];
	int thread_id[P];

	for (int i = 0; i < P; ++i)
		thread_id[i] = i;

	clock_t begin = clock();

	for (int i = 0; i < P; ++i)
		pthread_create(&(tid[i]), NULL, sqrt_f, &(thread_id[i]));

	for (int i = 0; i < P; ++i)
		pthread_join(tid[i], NULL);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent);
	return 0;
}