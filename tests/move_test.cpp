#include "hashtable.h"
#include "tests.h"

#include <string>
#include <string_view>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE);
    auto vval = make_rand_vec(VEC_SIZE, STR_SIZE);
    HashTable::HashTable<std::string, std::string_view> old_map;

    // Insert into map
    for (size_t i = 0; i < vkey.size(); i++)
    {
        const auto before = old_map.emplace(vkey[i], vval[i]);
        assert(!before.has_value()); // Assert there is no previous value
    }

    // Move into new map
    HashTable::HashTable<std::string, std::string_view> new_map = std::move(old_map);

    // Assert old map is empty
    assert(old_map.empty());
    assert(old_map.capacity() == 0);

    // Assert all keys & values can be found in the new map
    for (const auto &[k, v] : new_map.key_values())
    {
        // Find ptr in vec
        const auto ptr_key = std::find(vkey.begin(), vkey.end(), k);
        assert(ptr_key != vkey.cend());

        // Assert key & val are the same
        const ptrdiff_t pos = ptr_key - vkey.begin();
        assert(vkey[pos] == k);
        assert(vval[pos] == v);

        // Remove the element from the vector
        vkey.erase(vkey.begin() + pos);
        vval.erase(vval.begin() + pos);
    }

    // Assert both vecs are empty
    assert(vkey.empty());
    assert(vval.empty());
}