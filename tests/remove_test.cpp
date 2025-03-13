#include "hashtable.h"
#include "tests.h"

#include <string>
#include <string_view>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE);
    const auto vkey_wrong = make_rand_vec(VEC_SIZE, STR_SIZE, vkey);
    const auto vval = make_rand_vec(VEC_SIZE, STR_SIZE);
    HashTable::HashTable<std::string, std::string> m;

    // Insert value to map
    for (size_t i = 0; i < vval.size(); i++)
        m.emplace(vkey[i], vval[i]);

    // Assert removal with wrong key returns nothing
    for (size_t i = 0; i < vval.size(); i++)
    {
        const auto okv = m.remove(vkey_wrong[i]);
        assert(!okv.has_value());
    }

    // Assert removal with correct key returns original key & value
    for (size_t i = 0; i < vval.size(); i++)
    {
        const auto okv = m.remove(vkey[i]);
        assert(okv.has_value());
        const auto kv = okv.value();
        assert(kv.first == vkey[i]);
        assert(kv.second == vval[i]);
    }

    // Assert the same key values are removed
    for (size_t i = 0; i < vval.size(); i++)
    {
        const auto okv = m.remove(vkey[i]);
        assert(!okv.has_value());
    }
}