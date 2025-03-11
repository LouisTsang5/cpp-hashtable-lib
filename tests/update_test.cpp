#include "hashtable.h"
#include "tests.h"

#include <string>
#include <string_view>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    auto vkey = make_rand_vec(VEC_SIZE, STR_SIZE);
    const auto vval_before = make_rand_vec(VEC_SIZE, STR_SIZE);
    auto vval_after = make_rand_vec(VEC_SIZE, STR_SIZE);
    HashTable::HashTable<std::string, std::string_view> m;

    // Insert into map
    for (size_t i = 0; i < vkey.size(); i++)
    {
        const auto before = m.emplace(vkey[i], vval_before[i]);
        assert(!before.has_value()); // Assert there is no previous value
    }

    // Update the map
    for (size_t i = 0; i < vkey.size(); i++)
    {
        // Update new value and obtain old value
        const std::string &key = vkey[i];
        const std::string &new_val = vval_after[i];
        const auto before = m.emplace(key, new_val);

        // Assert the previous value is perserved
        assert(before.has_value());
        assert(before.value().first == key);
        assert(before.value().second == vval_before[i]);
    }

    // Assert all keys & values can be found
    for (const auto &slot : m.key_values())
    {
        // Find ptr in vec
        const auto ptr_key = std::find(vkey.begin(), vkey.end(), slot.ckey());
        assert(ptr_key != vkey.cend());

        // Assert key & val are the same
        const ptrdiff_t pos = ptr_key - vkey.begin();
        assert(vkey[pos] == slot.ckey());
        assert(vval_after[pos] == slot.cval());

        // Remove the element from the vector
        vkey.erase(vkey.begin() + pos);
        vval_after.erase(vval_after.begin() + pos);
    }

    // Assert both vecs are empty
    assert(vkey.empty());
    assert(vval_after.empty());
}