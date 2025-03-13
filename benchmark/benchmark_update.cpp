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

static void Map_Update_StringView(benchmark::State &state)
{
    size_t s = state.range(0);
    std::unordered_map<std::string_view, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, s);
    const auto voval = make_rand_vec(VEC_SIZE, s);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, s);
    }

    // Construct map
    for (size_t i = 0; i < vkey.size(); i++)
        m.emplace(vkey[i], voval[i]);

    size_t i = 0;
    for (auto _ : state)
    {
        std::vector<std::string> &vnval = new_val_vecs[i++ % new_val_vecs.size()];
        for (size_t i = 0; i < vkey.size(); i++)
        {
            auto p = m.find(vkey[i]);
            p->second = std::move(vnval[i]);
        }
    }
}
BM(Map_Update_StringView);

static void HashTable_Update_StringView(benchmark::State &state)
{
    size_t s = state.range(0);
    HashTable::HashTable<std::string_view, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, s);
    const auto voval = make_rand_vec(VEC_SIZE, s);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, s);
    }

    // Construct map
    for (size_t i = 0; i < vkey.size(); i++)
        m.emplace(vkey[i], voval[i]);

    size_t i = 0;
    for (auto _ : state)
    {
        std::vector<std::string> &vnval = new_val_vecs[i++ % new_val_vecs.size()];
        for (size_t i = 0; i < vkey.size(); i++)
            m.emplace(vkey[i], vnval[i]);
    }
}
BM(HashTable_Update_StringView);

static void Map_Update_String(benchmark::State &state)
{
    size_t s = state.range(0);
    std::unordered_map<std::string, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, s);
    const auto voval = make_rand_vec(VEC_SIZE, s);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, s);
    }

    // Construct map
    for (size_t i = 0; i < vkey.size(); i++)
        m.emplace(vkey[i], voval[i]);

    size_t i = 0;
    for (auto _ : state)
    {
        std::vector<std::string> &vnval = new_val_vecs[i++ % new_val_vecs.size()];
        for (size_t i = 0; i < vkey.size(); i++)
        {
            auto p = m.find(vkey[i]);
            p->second = std::move(vnval[i]);
        }
    }
}
BM(Map_Update_String);

static void HashTable_Update_String(benchmark::State &state)
{
    size_t s = state.range(0);
    HashTable::HashTable<std::string, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, s);
    const auto voval = make_rand_vec(VEC_SIZE, s);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, s);
    }

    // Construct map
    for (size_t i = 0; i < vkey.size(); i++)
        m.emplace(vkey[i], voval[i]);

    size_t i = 0;
    for (auto _ : state)
    {
        std::vector<std::string> &vnval = new_val_vecs[i++ % new_val_vecs.size()];
        for (size_t i = 0; i < vkey.size(); i++)
            m.emplace(vkey[i], vnval[i]);
    }
}
BM(HashTable_Update_String);

BENCHMARK_MAIN();