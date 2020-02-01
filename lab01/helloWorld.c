#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <limits.h>

typedef struct {int id;} test_t;

void* threadFunction(void *var)
{
	int thread_id = (*(test_t*)var).id;
	//for (int i = 0; i < 100; ++i)
		printf("Hello world from thread #1, core %i\n", thread_id);
}

void* threadFunction2(void *var)
{
	int thread_id = *(int*)var;
	printf("Hello world from thread #2, core %i\n", thread_id);
}

int main(int argc, char **argv)
{
	int P = 4;//sysconf(_SC_NPROCESSORS_ONLN);
	int i;

	pthread_t tid[P];
	int thread_id[P];
	test_t tests[P];

	for(i = 0;i < P; i++)
	{
		thread_id[i] = i;
		//tests[i].id = i;
	}
	for(i = 0; i < P; i++)
	{
		tests[i].id = i;
		pthread_create(&(tid[i]), NULL, threadFunction, &tests[i]);
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("%d\n", P);

	return 0;
}
