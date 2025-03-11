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
    HashTable::HashTable<std::string, std::string> m;

    // copy vectors
    const auto vkey_copy = vkey;
    const auto vval_copy = vval;

    // Move into map
    for (size_t i = 0; i < vkey.size(); i++)
    {
        m.emplace(vkey[i], vval[i]);
        assert(!vkey[i].empty() && vkey[i] == vkey_copy[i]);
        assert(!vval[i].empty() && vval[i] == vval_copy[i]);
    }
}