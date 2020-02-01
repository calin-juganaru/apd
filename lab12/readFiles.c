#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include <pthread.h>

#define n 1024*1024*1000

//dd if=/dev/urandom of=1.txt bs=1024 count=1024000
//dd if=/dev/urandom of=2.txt bs=1024 count=1024000

char buf1[n], buf2[n];

void* sqrt_f(void *var)
{
	int thread_id = *(int*)var;
	FILE* f1 = fopen("1.txt", "rb");
	fread(buf1, sizeof(char), n, f1);
	fclose(f1);
	FILE* f2 = fopen("2.txt","rb");
	fread(buf2, sizeof(char), n, f2);
	fclose(f2);
}

int P;

int main(int argc, char* argv[])
{
	pthread_t tid[P];
	int thread_id[P];

	for (int i = 0; i < P; ++i)
		thread_id[i] = i;

	for (int i = 0; i < P; ++i)
		pthread_create(&(tid[i]), NULL, sqrt_f, &(thread_id[i]));

	for (int i = 0; i < P; ++i)
		pthread_join(tid[i], NULL);

	return 0;
}