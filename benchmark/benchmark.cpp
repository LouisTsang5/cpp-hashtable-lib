#include "benchmark/benchmark.h"
#include "hashtable.h"

#include <array>
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
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_LongStringViewMapInserion);

static void BM_LongStringMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_LongStringMapInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_LongStringViewHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_LongStringViewHashTableInserion);

static void BM_LongStringHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_LongStringHashTableInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_ShortStringViewMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_ShortStringViewMapInserion);

static void BM_ShortStringMapInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        std::unordered_map<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_ShortStringMapInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_ShortStringViewHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string_view, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_ShortStringViewHashTableInserion);

static void BM_ShortStringHashTableInserion(benchmark::State &state)
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    for (auto _ : state)
    {
        std::vector<std::string> v_copy = v;
        HashTable::HashTable<std::string, std::string_view> m;
        for (size_t i = 0; i < v.size(); i++)
            m.emplace(std::move(v_copy[i]), v[i]);
    }
}
BENCHMARK(BM_ShortStringHashTableInserion);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_LongStringViewMapUpdate(benchmark::State &state)
{
    std::unordered_map<std::string_view, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto voval = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
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
BENCHMARK(BM_LongStringViewMapUpdate);

static void BM_LongStringMapUpdate(benchmark::State &state)
{
    std::unordered_map<std::string, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto voval = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
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
BENCHMARK(BM_LongStringMapUpdate);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_LongStringViewHashTableUpdate(benchmark::State &state)
{
    HashTable::HashTable<std::string_view, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto voval = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
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
BENCHMARK(BM_LongStringViewHashTableUpdate);

static void BM_LongStringHashTableUpdate(benchmark::State &state)
{
    HashTable::HashTable<std::string, std::string_view> m;
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    const auto voval = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);

    // Make new values array
    std::array<std::vector<std::string>, 32> new_val_vecs;
    for (auto &v : new_val_vecs)
    {
        v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
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
BENCHMARK(BM_LongStringHashTableUpdate);

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

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i % v1.size());
        const auto val = m.find(s);
        assert(val != m.end());
        assert(val->second == v2.at(i));
        i++;
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

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i % v1.size());
        const auto val = m.find(s);
        assert(val != m.end());
        assert(val->second == v2.at(i));
        i++;
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

    int i = 0;
    for (auto _ : state)
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

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i);
        const auto val = m.find(s);
        assert(val);
        assert(*val.value() == v2.at(i));
    }
}
BENCHMARK(BM_Long_String_HashTableLookup);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_Short_StringView_MapLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    std::unordered_map<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i % v1.size());
        const auto val = m.find(s);
        assert(val != m.end());
        assert(val->second == v2.at(i));
        i++;
    }
}
BENCHMARK(BM_Short_StringView_MapLookup);

static void BM_Short_String_MapLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    std::unordered_map<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i % v1.size());
        const auto val = m.find(s);
        assert(val != m.end());
        assert(val->second == v2.at(i));
        i++;
    }
}
BENCHMARK(BM_Short_String_MapLookup);

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

static void BM_Short_StringView_HashTableLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    HashTable::HashTable<std::string_view, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i);
        const auto val = m.find(s);
        assert(val);
        assert(*val.value() == v2.at(i));
    }
}
BENCHMARK(BM_Short_StringView_HashTableLookup);

static void BM_Short_String_HashTableLookup(benchmark::State &state)
{
    // Setup
    const auto v1 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    const auto v2 = make_rand_vec(VEC_SIZE, STR_SIZE_SHORT);
    HashTable::HashTable<std::string, std::string_view> m;
    for (size_t i = 0; i < v1.size(); i++)
    {
        const std::string &s1 = v1.at(i);
        const std::string &s2 = v2.at(i);
        m.emplace(s1, s2);
    }

    int i = 0;
    for (auto _ : state)
    {
        const std::string &s = v1.at(i);
        const auto val = m.find(s);
        assert(val);
        assert(*val.value() == v2.at(i));
    }
}
BENCHMARK(BM_Short_String_HashTableLookup);

BENCHMARK_MAIN();