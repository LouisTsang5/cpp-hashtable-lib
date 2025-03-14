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
    HashTable::HashTable<std::string, std::string_view> m;

    // Insert
    for (size_t i = 0; i < vkey.size(); i++)
    {
        const auto before = m.emplace(vkey[i], vval[i]);
        assert(m.size() == i + 1);         // Assert size increments after insert
        assert(m.occupancy() == m.size()); // Assert occupancy increments after insert
    }

    // Update
    const auto vval_new = make_rand_vec(VEC_SIZE, STR_SIZE, vval);
    for (size_t i = 0; i < vkey.size(); i++)
    {
        const auto before = m.emplace(vkey[i], vval[i]);
        assert(m.size() == vkey.size());   // Assert size remains the same after update
        assert(m.occupancy() == m.size()); // Assert occupancy remains the same after update
    }

    // Delete
    for (size_t i = 0; i < vkey.size(); i++)
    {
        const auto before = m.remove(vkey[i]);
        assert(m.size() == vkey.size() - i - 1); // Assert size decrements after delete
        assert(m.occupancy() == vkey.size());    // Assert occupancy remains the same after delete
    }
}