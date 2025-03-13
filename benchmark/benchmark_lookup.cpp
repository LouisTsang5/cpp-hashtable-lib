#include "benchmark/benchmark.h"
#include "hashtable.h"
#include "bm.h"

#include <array>
#include <string_view>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <cassert>
#include <vector>

static void Map_Lookup_StringView(benchmark::State &state)
{
    // Setup
    size_t s = state.range(0);
    const auto v1 = make_rand_vec(VEC_SIZE, s);
    const auto v2 = make_rand_vec(VEC_SIZE, s);
    std::unordered_map<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < v1.size(); i++)
        {
            const auto val = m.find(v1[i]);
            assert(val != m.end());
            assert(val->second == v2[i]);
        }
    }
}
BM(Map_Lookup_StringView);

static void HashTable_Lookup_StringView(benchmark::State &state)
{
    // Setup
    size_t s = state.range(0);
    const auto v1 = make_rand_vec(VEC_SIZE, s);
    const auto v2 = make_rand_vec(VEC_SIZE, s);
    HashTable::HashTable<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < v1.size(); i++)
        {
            const auto val = m.find(v1[i]);
            assert(val);
            assert(*val.value() == v2[i]);
        }
    }
}
BM(HashTable_Lookup_StringView);

static void Map_Lookup_String(benchmark::State &state)
{
    // Setup
    size_t s = state.range(0);
    const auto v1 = make_rand_vec(VEC_SIZE, s);
    const auto v2 = make_rand_vec(VEC_SIZE, s);
    std::unordered_map<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < v1.size(); i++)
        {
            const auto val = m.find(v1[i]);
            assert(val != m.end());
            assert(val->second == v2[i]);
        }
    }
}
BM(Map_Lookup_String);

static void HashTable_Lookup_String(benchmark::State &state)
{
    // Setup
    size_t s = state.range(0);
    const auto v1 = make_rand_vec(VEC_SIZE, s);
    const auto v2 = make_rand_vec(VEC_SIZE, s);
    HashTable::HashTable<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    for (auto _ : state)
    {
        for (size_t i = 0; i < v1.size(); i++)
        {
            const auto val = m.find(v1[i]);
            assert(val);
            assert(*val.value() == v2[i]);
        }
    }
}
BM(HashTable_Lookup_String);

BENCHMARK_MAIN();