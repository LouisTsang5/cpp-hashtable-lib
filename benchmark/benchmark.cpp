#include "benchmark/benchmark.h"
#include "hashtable.h"

#include <string_view>
#include <cstdint>
#include <string>
#include <random>
#include <unordered_map>
#include <cassert>
#include <vector>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE_LONG = 31;
constexpr size_t STR_SIZE_SHORT = 8;

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

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void BM_LongStringViewMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    std::unordered_map<std::string_view, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_LongStringViewMapInserion);

static void BM_LongStringMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    std::unordered_map<std::string, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_LongStringMapInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_ShortStringViewMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    std::unordered_map<std::string_view, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_ShortStringViewMapInserion);

static void BM_ShortStringMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    std::unordered_map<std::string, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_ShortStringMapInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_ShortStringViewHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    HashTable::HashTable<std::string_view, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_ShortStringViewHashTableInserion);

static void BM_ShortStringHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    HashTable::HashTable<std::string, std::string_view> m;
    for (auto _ : state)
        for (const std::string &s : v)
            m.emplace(s, s);
}
BENCHMARK(BM_ShortStringHashTableInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_Long_StringView_MapLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    std::unordered_map<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
        for (size_t i = 0; i < v1.size(); i++)
        {
            const std::string &s = v1.at(i);
            const auto val = m.find(s);
            assert(val != m.end());
            assert((*val).second == v2.at(i));
        }
}
BENCHMARK(BM_Long_StringView_MapLookup);

static void BM_Long_String_MapLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    std::unordered_map<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
        for (size_t i = 0; i < v1.size(); i++)
        {
            const std::string &s = v1.at(i);
            const auto val = m.find(s);
            assert(val != m.end());
            assert((*val).second == v2.at(i));
        }
}
BENCHMARK(BM_Long_String_MapLookup);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_Long_StringView_HashTableLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    HashTable::HashTable<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
        for (size_t i = 0; i < v1.size(); i++)
        {
            const std::string &s = v1.at(i);
            const auto val = m.find(s);
            assert(val);
            assert(*val.value() == v2.at(i));
        }
}
BENCHMARK(BM_Long_StringView_HashTableLookup);

static void BM_Long_String_HashTableLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    HashTable::HashTable<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
        for (size_t i = 0; i < v1.size(); i++)
        {
            const std::string &s = v1.at(i);
            const auto val = m.find(s);
            assert(val);
            assert(*val.value() == v2.at(i));
        }
}
BENCHMARK(BM_Long_String_HashTableLookup);

BENCHMARK_MAIN();