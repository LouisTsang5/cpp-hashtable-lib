#include "hashtable.h"
#include "tests.h"

#include <string>
#include <string_view>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    const auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE);
    const auto vval = make_rand_vec(VEC_SIZE, STR_SIZE);
    HashTable::HashTable<std::string, std::string> m;

    // Move into map
    auto vkey_copy1 = vkey;
    auto vval_copy1 = vval;
    for (size_t i = 0; i < vkey_copy1.size(); i++)
    {
        m.emplace(std::move(vkey_copy1[i]), std::move(vval_copy1[i]));
        assert(vkey_copy1[i].empty()); // Assert key is moved
        assert(vval_copy1[i].empty()); // Assert value is moved
    }

    // Moving of key does not happen if the key is already in the map
    auto vkey_copy2 = vkey;
    auto vval_copy2 = vval;
    for (size_t i = 0; i < vkey_copy2.size(); i++)
    {
        m.emplace(std::move(vkey_copy2[i]), std::move(vval_copy2[i]));
        assert(!vkey_copy2[i].empty());   // Assert key is not move
        assert(vkey_copy2[i] == vkey[i]); // Assert key is still the same
        assert(vval_copy2[i].empty());    // Assert value is moved
    }
}