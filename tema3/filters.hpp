#pragma once

#include <iostream>
#include <vector>

using namespace std;

// ================================================================================================

template <typename T1, typename T2, typename T3>
struct triplet
{
    T1 first; T2 second; T3 third;

    triplet() = default;
    ~triplet() = default;

    template <typename U1, typename U2, typename U3>
    triplet(U1&& _first, U2&& _second, U3&& _third):
        first(forward<U1>(_first)),
        second(forward<U2>(_second)),
        third(forward<U3>(_third)) {}

    template <typename U1, typename U2, typename U3>
    triplet& operator=(const triplet<U1, U2, U3>& other)
    {
        first = other.first;
        second = other.second;
        third = other.third;
        return *this;
    }

    template <typename U1, typename U2, typename U3>
    triplet operator+(const triplet<U1, U2, U3>& other) const
    {
        return
        {
            first + other.first,
            second + other.second,
            third + other.third
        };
    }

    template <typename U1, typename U2, typename U3>
    triplet& operator+=(const triplet<U1, U2, U3>& other)
    {
        first += other.first;
        second += other.second;
        third += other.third;
        return *this;
    }

    template <typename T>
    triplet<T, T, T> operator*(const T& other) const
    {
        return {other * first, other * second, other * third};
    }
};

// ================================================================================================

using itrip_t = triplet<uint8_t, uint8_t, uint8_t>;
using ftrip_t = triplet<float, float, float>;

using ivec_t = vector<itrip_t>;
using fvec_t = vector<float>;

using image_t = vector<ivec_t>;
using filter_t = vector<fvec_t>;

// ================================================================================================

inline void compute_filter(image_t& image, const filter_t& filter)
{
    auto N = image.size();
    auto M = image.front().size();
    auto result = image_t(N, ivec_t(M));

    for (auto i = 1u; i < N - 1u; ++i)
        for (auto j = 1u; j < M - 1u; ++j)
        {
            auto aux = ftrip_t(0.0f, 0.0f, 0.0f);

            for (auto x: {-1, 0, 1})
                for (auto y: {-1, 0, 1})
                    aux += image[i + x][j + y] * filter[1 - x][1 - y];

            if (aux.first  < 0) aux.first  = 0;
            if (aux.second < 0) aux.second = 0;
            if (aux.third  < 0) aux.third  = 0;

            if (aux.first  > 255) aux.first  = 255;
            if (aux.second > 255) aux.second = 255;
            if (aux.third  > 255) aux.third  = 255;

            result[i][j] = aux;
        }

    image = move(result);
}

// ================================================================================================

inline void smoothing(image_t& image)
{
    compute_filter(image, filter_t(3u, fvec_t(3u, 1.0f / 9.0f)));
}

// ================================================================================================

inline void gauss_blur(image_t& image)
{
    compute_filter(image,
    {
        {1.0f / 16.0f, 1.0f / 8.0f, 1.0f / 16.0f},
        {1.0f / 8.0f,  1.0f / 4.0f, 1.0f / 8.0f},
        {1.0f / 16.0f, 1.0f / 8.0f, 1.0f / 16.0f}
    });
}

// ================================================================================================

inline void sharpen(image_t& image)
{
    compute_filter(image,
    {
        {0, -2.0f / 3.0f, 0},
        {-2.0f / 3.0f, 11.0f / 3.0f, -2.0f / 3.0f},
        {0, -2.0f / 3.0f, 0}
    });
}

// ================================================================================================

inline void mean_removal(image_t& image)
{
    auto filter = filter_t(3u, fvec_t(3u, -1.0f));
    filter[1][1] = 9.0f;
    compute_filter(image, filter);
}

// ================================================================================================

inline void emboss(image_t& image)
{
    auto filter = filter_t(3u, fvec_t(3u, 0.0f));
    filter[0][1] = 1.0f; filter[2][1] = -1.0f;
    compute_filter(image, filter);
}

// ================================================================================================

inline void apply_filter(image_t& image, const string& filter_name)
{
    if (filter_name == "smooth")
        smoothing(image);
    else if (filter_name == "blur")
        gauss_blur(image);
    else if (filter_name == "sharpen")
        sharpen(image);
    else if (filter_name == "mean")
        mean_removal(image);
    else if (filter_name == "emboss")
        emboss(image);
    else cout << "Invalid filter!";
}

// ================================================================================================