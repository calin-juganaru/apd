#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

int n = 1024*1024*100;
int a;
int P;

void* sqrt_f(void* var)
{
	int c, start, stop, thread_id = *(int*)var;
	start = thread_id * (n / P);
	stop = (thread_id + 1) * (n / P) + (thread_id == P - 1 ? n % P : 0);

	for (int i  = start; i < stop; ++i)
		c = sqrt(a);
}

int main(int argc, char * argv[])
{
	clock_t begin = clock();

	P = atoi(argv[1]);

	pthread_t tid[P];
	int thread_id[P];

	for (int i = 0; i < P; ++i)
		thread_id[i] = i;

	for (int i = 0; i < P; ++i)
		pthread_create(&(tid[i]), NULL, sqrt_f, &(thread_id[i]));

	for (int i = 0; i < P; ++i)
		pthread_join(tid[i], NULL);

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n", time_spent);
	return 0;
}