#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int a;
pthread_mutex_t mutex;
pthread_barrier_t barrier;

void* threadFunction(void *var)
{
	int thread_id = *(int*)var;
	printf("Hello world from thread %i\n", thread_id);
}

void* threadFunction1(void *var)
{
	int thread_id = *(int*)var;
	printf("Hello world 0 from thread %i\n", thread_id);
	pthread_barrier_wait(&barrier);
}

void* threadFunction2(void *var)
{
	int thread_id = *(int*)var;
	pthread_barrier_wait(&barrier);
	printf("Hello world 1 from thread %i\n", thread_id);
}

int main(int argc, char **argv)
{
	int P = 2;
	int i;
	//pthread_mutex_init(&mutex, NULL);
	pthread_barrier_init(&barrier, NULL, P);

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	//for(i = 0; i < P; i++)
	//{
		pthread_create(&(tid[0]), NULL, threadFunction1, &(thread_id[0]));
		pthread_create(&(tid[1]), NULL, threadFunction2, &(thread_id[1]));
	//}

	for(i = 0; i < P; i++)
	{
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);
	//pthread_mutex_destroy(&mutex);
	return 0;
}
