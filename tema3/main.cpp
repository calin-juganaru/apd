#include <fstream>

#include <mpi.h>

#include "./filters.hpp"

auto PNM = false;
auto N = 0u, M = 0u;
auto MAX_VAL = 0;

auto image = image_t();

// ================================================================================================

void parse_input(const string& input_file)
{
    auto input = ifstream();
    input.open(input_file, ios_base::binary);

    {
        auto aux = string();
        getline(input, aux);
        PNM = (aux[1] == '6');

        getline(input, aux);
        input >> aux; M = stoi(aux);
        input >> aux; N = stoi(aux);
        input >> aux; MAX_VAL = stoi(aux);

        image.resize(N + 2u);
        for (auto& line: image)
            line.resize(M + 2u);

        getline(input, aux);
    }

    if (PNM)
    {
        for (auto i = 1u; i <= N; ++i)
        {
            auto buffer = vector<char>(3u * M);
            input.read(buffer.data(), buffer.size());

            for (auto j = 3u; j <= buffer.size(); j += 3u)
                image[i][j / 3u] =
                {
                    move(buffer[j - 3u]),
                    move(buffer[j - 2u]),
                    move(buffer[j - 1u])
                };
        }
    }
    else
    {
        for (auto i = 1u; i <= N; ++i)
        {
            auto buffer = vector<char>(M);
            input.read(buffer.data(), buffer.size());

            for (auto j = 1u; j <= buffer.size(); ++j)
                image[i][j].first = move(buffer[j - 1u]);
        }
    }
}

// ================================================================================================

inline void write_image(const string& output_file)
{
    auto output = ofstream();
    auto N = image.size() - 2u;
    auto M = image.front().size() - 2u;
    output.open(output_file, ios_base::binary);
    output << 'P' << (PNM ? 6 : 5) << "\n# \n";
    output << M << ' ' << N << endl << MAX_VAL << endl;

    if (PNM)
    {
        for (auto i = 1u; i <= N; ++i)
            for (auto j = 1u; j <= M; ++j)
                output << image[i][j].first
                       << image[i][j].second
                       << image[i][j].third;
    }
    else
    {
        for (auto i = 1u; i <= N; ++i)
            for (auto j = 1u; j <= M; ++j)
                output << image[i][j].first;
    }
}

// ================================================================================================

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int thread_id, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    P = max (P, 1);

    auto input_file = string(argv[1]);
    auto output_file = string(argv[2]);

    if (!thread_id)
    {
        parse_input(input_file);

        MPI_Datatype NEW_TYPE;
        MPI_Type_vector(M + 2u, 3u, 3u, MPI_CHAR, &NEW_TYPE);
        MPI_Type_commit(&NEW_TYPE);

        for (auto filter_id = 3; filter_id < argc; ++filter_id)
        {
            auto filter_name = string(argv[filter_id]);

            for (auto id = 1; id < P; ++id)
            {
                MPI_Send(&M, 1, MPI_UNSIGNED, id, 0, MPI_COMM_WORLD);
                MPI_Send(&N, 1, MPI_UNSIGNED, id, 1, MPI_COMM_WORLD);

                auto size = N / P;
                auto start = id * size;
                auto stop = start + size;

                if (id == P - 1)
                    stop += N % P + 1;

                for (auto i = start - 1; i <= stop; ++i)
                    MPI_Send(image[i].data(), 1, NEW_TYPE, id, i, MPI_COMM_WORLD);
            }

            while (image.size() > 1 + N / P)
                image.pop_back();
            apply_filter(image, filter_name);
            image.pop_back();

            for (auto id = 1; id < P; ++id)
            {
                auto size = N / P;
                auto start = id * size;
                auto stop = start + size;

                if (id == P - 1)
                    stop += N % P + 1;

                for (auto i = start; i < stop; ++i)
                {
                    auto recv = ivec_t(M + 2);
                    MPI_Recv(recv.data(), 1, NEW_TYPE, id, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    image.push_back(move(recv));
                }
            }

            image.push_back(ivec_t(M + 2));
        }
    }
    else
    {
        for (auto filter_id = 3; filter_id < argc; ++filter_id)
        {
            auto filter_name = string(argv[filter_id]);

            MPI_Recv(&M, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&N, 1, MPI_UNSIGNED, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Datatype NEW_TYPE;
            MPI_Type_vector(M + 2u, 3u, 3u, MPI_CHAR, &NEW_TYPE);
            MPI_Type_commit(&NEW_TYPE);

            auto size = N / P;
            auto start = thread_id * size;
            auto stop = start + size;

            if (thread_id == P - 1)
                stop += N % P + 1;

            for (auto i = start - 1; i <= stop; ++i)
            {
                auto recv = ivec_t(M + 2);
                MPI_Recv(recv.data(), 1, NEW_TYPE, 0, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                image.push_back(move(recv));
            }

            apply_filter(image, filter_name);

            for (auto i = start; i < stop; ++i)
                MPI_Send(image[i - start + 1].data(), 1, NEW_TYPE, 0, i, MPI_COMM_WORLD);

            image.clear();
        }
    }

    if (!thread_id)
        write_image(output_file);

    MPI_Finalize();
}