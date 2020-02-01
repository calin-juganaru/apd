#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define newline printf("\n");

typedef double complex Complex;

const double epsilon = 0.0000000000001;
const double PI = 3.14159265358979323846264338328;

pthread_barrier_t barrier;

typedef struct
{
    int id, N, P;
    Complex* v;
} args_t;

Complex* read_input(int* N, char* file)
{
    freopen(file, "r", stdin);
    scanf("%d", N);
    Complex* v = calloc(*N, sizeof(Complex));

    for (int i = 0; i < *N; ++i)
    {
        double aux;
        scanf("%lf", &aux);
        v[i] = aux;
    }

    return v;
}

void print_output(Complex* v, int N, char* file)
{
 	freopen(file, "w", stdout);
    printf("%i\n", N);
	for (int i = 0; i < N; ++i)
		printf("%0.6lf %0.6lf\n", creal(v[i]), cimag(v[i]));
}

void fourier(Complex v1[], int N)
{
    Complex v2[N];

    for (int k = 0; k < N; ++k)
    {
        v2[k] = 0;
        for (int j = 0; j < N; ++j)
            v2[k] += v1[j] * cexp(-2 * PI * I * k * j / N);

        if (fabs(cimag(v2[k])) < epsilon)
            v2[k] = creal(v2[k]);

        if (fabs(creal(v2[k])) < epsilon)
            v2[k] = 0;
    }

    for (int i = 0; i < N; ++i)
        v1[i] = v2[i];
}

void* parallel_fourier(void* _args)
{
    args_t args = *(args_t*)_args;
    int thread_id = args.id;
	int size = args.N / args.P;
	int start = size * thread_id;
	int end = start + size;
    Complex* v2 = calloc(args.N, sizeof(Complex));

    for (int k = start; k < end; ++k)
    {
        v2[k] = 0;
        for (int j = 0; j < args.N; ++j)
        {
            v2[k] += args.v[j] * cexp(-2 * PI * I * k * j / args.N);
        }

        if (fabs(cimag(v2[k])) < epsilon)
            v2[k] = creal(v2[k]);

        if (fabs(creal(v2[k])) < epsilon)
            v2[k] = 0;
    }

    pthread_barrier_wait(&barrier);

    for (int i = start; i < end; ++i)
        args.v[i] = v2[i];

    free(v2);
    return NULL;
}

int main(int argc, char* argv[])
{
	int N, P = atoi(argv[3]);
    Complex* v = read_input(&N, argv[1]);

    pthread_t thread_id[P];
	args_t args[P];
    pthread_barrier_init(&barrier, NULL, P);

    for (int i = 0; i < P; ++i)
	{
		args[i].N = N;
        args[i].P = P;
		args[i].v = v;
		args[i].id = i;
	}

	for (int i = 0; i < P; ++i)
        pthread_create(&(thread_id[i]), NULL, parallel_fourier, &args[i]);

    for (int i = 0; i < P; ++i)
		pthread_join(thread_id[i], NULL);

    print_output(v, N, argv[2]);
    pthread_barrier_destroy(&barrier);
    free(v);
	return 0;
}