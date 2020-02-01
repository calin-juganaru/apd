#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

typedef double complex Complex;

const double PI = 3.14159265358979323846264338328;

typedef struct
{
    int step, N;
    Complex* v1;
	Complex* v2;
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

void* fft_aux(void* _args)
{
	args_t args = *(args_t*)_args;
	int step = args.step;
	int N = args.N;
	Complex* v1 = args.v1;
	Complex* v2 = args.v2;

	if (step < N)
	{
		args.step = step * 2;
		args.v1 = v2;
		args.v2 = v1;
		fft_aux(&args);

		args.v1 = v2 + step;
		args.v2 = v1 + step;
		fft_aux(&args);

		for (int i = 0; i < N; i += 2 * step)
		{
			Complex t = cexp(-I * PI * i / N) * v2[i + step];
			v1[i / 2]     = v2[i] + t;
			v1[(i + N)/2] = v2[i] - t;
		}
	}

	return NULL;
}

void parallel_fast_fourier(Complex v1[], int N, int P)
{
	Complex* v2 = calloc(N, sizeof(Complex));
	for (int i = 0; i < N; ++i)
        v2[i] = v1[i];

    pthread_t thread_id[P];
	args_t args[P];

    for (int i = 0; i < P; ++i)
	{
		args[i].N = N;
		args[i].v1 = v1;
		args[i].v2 = v2;
		args[i].step = P;
	}

	switch (P)
	{
		case 1:
			fft_aux(&args[0]);
			break;

		case 2:
			for (int i = 0; i < 2; ++i)
			{
				args[i].v2 = v1 + i % 2;
				args[i].v1 = v2 + i % 2;
				pthread_create(&(thread_id[i]), NULL, fft_aux, &args[i]);
			}
			for (int i = 0; i < 2; ++i)
				pthread_join(thread_id[i], NULL);

			for (int i = 0; i < N; i += 2)
			{
				Complex t = cexp(-I * PI * i / N) * v2[i + 1];
				v1[i / 2]     = v2[i] + t;
				v1[(i + N)/2] = v2[i] - t;
			}

			break;

		case 4:
			args[0].v1 = v1;
			args[0].v2 = v2;
			args[1].v1 = v1 + 2;
			args[1].v2 = v2 + 2;
			args[2].v1 = v1 + 1;
			args[2].v2 = v2 + 1;
			args[3].v1 = v1 + 3;
			args[3].v2 = v2 + 3;

			for (int i = 0; i < 2; ++i)
				pthread_create(&(thread_id[i]), NULL, fft_aux, &args[i]);

			for (int i = 0; i < 2; ++i)
				pthread_join(thread_id[i], NULL);

			for (int i = 0; i < N; i += 4)
			{
				Complex t = cexp(-I * PI * i / N) * v1[i + 2];
				v2[i / 2]     = v1[i] + t;
				v2[(i + N)/2] = v1[i] - t;
			}

			for (int i = 2; i < 4; ++i)
				pthread_create(&(thread_id[i]), NULL, fft_aux, &args[i]);

			for (int i = 2; i < 4; ++i)
				pthread_join(thread_id[i], NULL);

			for (int i = 0; i < N; i += 4)
			{
				Complex t = cexp(-I * PI * i / N) * v1[1 + i + 2];
				v2[1 + i / 2]     = v1[1 + i] + t;
				v2[1 + (i + N)/2] = v1[1 + i] - t;
			}

			for (int i = 0; i < N; i += 2)
			{
				Complex t = cexp(-I * PI * i / N) * v2[i + 1];
				v1[i / 2]     = v2[i] + t;
				v1[(i + N)/2] = v2[i] - t;
			}

			break;

		default: break;
	}

	free(v2);
}

void fast_fourier(Complex v[], int N)
{
	unsigned k = N, n, l, i, j;
	Complex expn = cexp(-2 * I * PI / N);
    Complex T, aux;

    while (k > 1)
	{
		n = k;
		k >>= 1;

		for (l = 0, T = 1; l < k; ++l)
		{
			for (i = l; i < N; i += n)
			{
				j = i + k;
				aux = v[i] - v[j];
				v[i] += v[j];
				v[j] = aux * T;
			}

			T *= expn;
		}

        expn = expn * expn;
	}

	unsigned log2_N = log2(N);
	for (i = j = 0; i < N; ++i, j = i)
	{
		j = (((j & 0xaaaaaaaa) >> 1) | ((j & 0x55555555) << 1));
		j = (((j & 0xcccccccc) >> 2) | ((j & 0x33333333) << 2));
		j = (((j & 0xf0f0f0f0) >> 4) | ((j & 0x0f0f0f0f) << 4));
		j = (((j & 0xff00ff00) >> 8) | ((j & 0x00ff00ff) << 8));
		j = ((j >> 16) | (j << 16)) >> (32 - log2_N);

        if (j > i)
		{
			aux = v[i];
			v[i] = v[j];
			v[j] = aux;
		}
	}
}

int main(int argc, char* argv[])
{
	int N, P = atoi(argv[3]);
    Complex* v = read_input(&N, argv[1]);
	parallel_fast_fourier(v, N, P);
	print_output(v, N, argv[2]);
	free(v);
	return 0;
}