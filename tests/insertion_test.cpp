#include "hashtable.h"
#include "tests.h"

#include <string_view>
#include <optional>

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE = 32;

int main()
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE);
    std::vector<NoCopyString> v_copy;
    for (const std::string &s : v)
        v_copy.push_back(s);

    HashTable::HashTable<std::string, std::string_view> m;
    for (size_t i = 0; i < v.size(); i++)
    {
        const std::string &cstr = v[i];
        std::string &str = v_copy[i];
        m.emplace(std::move(str), cstr);
    }

    // Assert values can be found
    for (const auto &slot : m.key_values())
    {
        const auto key = std::find(v.begin(), v.end(), slot.ckey());
        assert(key != v.cend());
        assert(*key == slot.cval());
    }

    // Assert values are moved
    for (const std::string &s : v_copy)
    {
        assert(s.empty());
    }
}