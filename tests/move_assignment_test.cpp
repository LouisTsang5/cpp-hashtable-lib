#include "hashtable.h"
#include "tests.h"

#include <string>
#include <string_view>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    auto vkey1 = make_rand_vec(VEC_SIZE, STR_SIZE);
    auto vval1 = make_rand_vec(VEC_SIZE, STR_SIZE);
    HashTable::HashTable<std::string, std::string_view> map1;
    for (size_t i = 0; i < vkey1.size(); i++)
    {
        const auto before = map1.emplace(vkey1[i], vval1[i]);
        assert(!before.has_value()); // Assert there is no previous value
    }

    auto vkey2 = make_rand_vec(VEC_SIZE * 2, STR_SIZE, vkey1);
    auto vval2 = make_rand_vec(VEC_SIZE * 2, STR_SIZE, vval1);
    HashTable::HashTable<std::string, std::string_view> map2;
    for (size_t i = 0; i < vkey2.size(); i++)
    {
        const auto before = map2.emplace(vkey2[i], vval2[i]);
        assert(!before.has_value()); // Assert there is no previous value
    }

    // Assert both maps have different sizes
    assert(map1.size() * 2 == map2.size());
    assert(map1.capacity() != map2.capacity());

    // move assignment
    size_t prev_map1_cap = map1.capacity();
    map2 = std::move(map1);

    // Assert map1 is empty
    assert(map1.empty());
    assert(map1.capacity() == 0);

    // Assert map2 has the size of former map1
    assert(map2.size() == vkey1.size());
    assert(map2.capacity() == prev_map1_cap);

    // Assert no key1's & value1's can be found in map1
    {
        auto vkey = vkey1;
        auto vval = vval1;
        for (const auto &[k, v] : map1.key_values())
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
        assert(vkey.size() == vkey1.size());
        assert(vval.size() == vval1.size());
    }

    // Assert all key1's & value1's can be found in map2
    {
        auto vkey = vkey1;
        auto vval = vval1;
        for (const auto &[k, v] : map2.key_values())
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

    // Assert no key2's & value2's can be found in map2
    {
        auto vkey = vkey2;
        auto vval = vval2;
        for (const auto &[k, v] : map2.key_values())
        {
            assert(std::find(vkey.begin(), vkey.end(), k) == vkey.cend());
            assert(std::find(vval.begin(), vval.end(), v) == vval.cend());
        }
    }
}