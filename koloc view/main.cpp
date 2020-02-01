#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct args_t
{
    int thread_id, i, k;

    args_t() = default;
    args_t(int t, int i, int k):
        thread_id(t), i(i), k(k) {}
};

constexpr auto N = 5;
auto P = 1;

int graph[N][N] =
{
    { 0, 3, 9, 8, 3 },
    { 5, 0, 1, 4, 2 },
    { 6, 6, 0, 4, 5 },
    { 2, 9, 2, 0, 7 },
    { 7, 9, 3, 2, 0 }
};

void* roy_floyd(void* _args)
{
	auto args = *(args_t*)_args;
    auto [thread_id, i, k] = args;

	auto begin = (N / P) * thread_id;
	auto end = begin + N / P;

	if (thread_id == P - 1)
		end += N % P;

    for (int j = begin; j < end; j++)
		graph[i][j] = min(graph[i][k] + graph[k][j], graph[i][j]);

    return nullptr;
}

int main(int argc, char* argv[])
{
	P = stoi(argv[1]);

    pthread_t thread[P];
	int thread_id[P];

	for (auto i = 0; i < P; ++i)
		thread_id[i] = i;

    for (auto k = 0; k < N; ++k)
		for (auto I = 0; I < N; ++I)
		{
			auto args = vector<args_t>(P);
            for (auto i = 0; i < P; ++i)
		        args[i] = args_t(i, I, k);

            for (auto i = 0; i < P; ++i)
                pthread_create(&(thread[0]), nullptr, roy_floyd, &args[i]);

	        for (auto i = 0; i < P; ++i)
		        pthread_join(thread[i], nullptr);
        }

    for (auto i = 0; i < N; ++i)
	{
		for (auto j = 0; j < N; ++j)
			cout << graph[i][j] << " ";
		cout << endl;
	}
}