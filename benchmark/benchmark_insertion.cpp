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

static void Map_Insertion_StringView(benchmark::State &state)
{
    size_t s = state.range(0);
    const auto v = make_rand_vec(VEC_SIZE, s);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BM(Map_Insertion_StringView);

static void HashTable_Insertion_StringView(benchmark::State &state)
{
    size_t s = state.range(0);
    const auto v = make_rand_vec(VEC_SIZE, s);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BM(HashTable_Insertion_StringView);

static void Map_Insertion_String(benchmark::State &state)
{
    size_t s = state.range(0);
    const auto v = make_rand_vec(VEC_SIZE, s);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BM(Map_Insertion_String);

static void HashTable_Insertion_String(benchmark::State &state)
{
    size_t s = state.range(0);
    const auto v = make_rand_vec(VEC_SIZE, s);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BM(HashTable_Insertion_String);

BENCHMARK_MAIN();