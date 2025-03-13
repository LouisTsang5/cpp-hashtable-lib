
#include "benchmark/benchmark.h"

#include <cstdint>
#include <random>
#include <string>
#include <vector>

constexpr size_t VEC_SIZE = 256;
#define BM(bm) BENCHMARK(bm)->Arg(8 - 1)->Arg(16 - 1)->Arg(32 - 1)->Arg(64 - 1)->Arg(128 - 1)

std::string generateRandomString(size_t size)
{
    const std::string_view characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.length() - 1);

    std::string randomString;
    randomString.reserve(size);

    for (size_t i = 0; i < size; i++)
    {
        randomString += characters[distribution(generator)];
    }

    return randomString;
}

std::vector<std::string> make_rand_vec(size_t vec_size, size_t str_size)
{
    std::vector<std::string> v;
    v.reserve(vec_size);
    for (size_t i = 0; i < vec_size; i++)
    {
        v.push_back(generateRandomString(str_size));
    }
    return v;
}