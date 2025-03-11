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

    // Insert value to map
    for (size_t i = 0; i < vval.size(); i++)
        m.emplace(vkey[i], vval[i]);

    // Assert lookup works
    for (size_t i = 0; i < vval.size(); i++)
    {
        const std::optional<std::string *> val = m.find(vkey[i]);
        assert(val.has_value());
        assert(*val.value() == vval[i]);
    }
}