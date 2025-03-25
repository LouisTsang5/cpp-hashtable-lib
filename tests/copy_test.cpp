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

    // Copy to new map
    HashTable::HashTable<std::string, std::string_view> new_map = old_map;

    // Assert old map size is the same as new map
    assert(old_map.size() == new_map.size());
    assert(old_map.capacity() == new_map.capacity());

    // Assert all keys & values can be found in the old map
    auto vkey_old = vkey;
    auto vval_old = vval;
    for (const auto &[k, v] : old_map.key_values())
    {
        // Find ptr in vec
        const auto ptr_key = std::find(vkey_old.begin(), vkey_old.end(), k);
        assert(ptr_key != vkey_old.cend());

        // Assert key & val are the same
        const ptrdiff_t pos = ptr_key - vkey_old.begin();
        assert(vkey_old[pos] == k);
        assert(vval_old[pos] == v);

        // Remove the element from the vector
        vkey_old.erase(vkey_old.begin() + pos);
        vval_old.erase(vval_old.begin() + pos);
    }

    // Assert both vecs are empty
    assert(vkey_old.empty());
    assert(vval_old.empty());

    // Assert all keys & values can be found in the new map
    auto vkey_new = vkey;
    auto vval_new = vval;
    for (const auto &[k, v] : new_map.key_values())
    {
        // Find ptr in vec
        const auto ptr_key = std::find(vkey_new.begin(), vkey_new.end(), k);
        assert(ptr_key != vkey_new.cend());

        // Assert key & val are the same
        const ptrdiff_t pos = ptr_key - vkey_new.begin();
        assert(vkey_new[pos] == k);
        assert(vval_new[pos] == v);

        // Remove the element from the vector
        vkey_new.erase(vkey_new.begin() + pos);
        vval_new.erase(vval_new.begin() + pos);
    }

    // Assert both vecs are empty
    assert(vkey_new.empty());
    assert(vval_new.empty());
}