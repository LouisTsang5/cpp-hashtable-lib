#include "/hashtable.h"

#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

std::string generateRandomString(size_t size)
{
    const std::string_view characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, characters.length() - 1);

    std::string randomString;
    randomString.reserve(size);

    for (size_t i = 0; i < size; i++)
    {
        randomString += characters[distribution(generator)];
    }

    return randomString;
}

std::vector<std::string> make_rand_vec(size_t vec_size, size_t str_size)
{
    std::vector<std::string> v;
    v.reserve(vec_size);
    for (size_t i = 0; i < vec_size; i++)
    {
        v.push_back(generateRandomString(str_size));
    }
    return v;
}

constexpr size_t VEC_SIZE = 256;
constexpr size_t STR_SIZE_LONG = 32;
constexpr size_t STR_SIZE_SHORT = 32;

class NoCopyString : public std::string
{
public:
    NoCopyString(const std::string &s) noexcept : std::string(s) {}
    NoCopyString(const NoCopyString &) = delete;
    NoCopyString &operator=(const NoCopyString &) = delete;
    NoCopyString(NoCopyString &&) = default;
    NoCopyString &operator=(NoCopyString &&) = default;
};

int main()
{
    const auto v = make_rand_vec(VEC_SIZE, STR_SIZE_LONG);
    std::vector<NoCopyString> v_copy;
    for (const std::string &s : v)
    {
        v_copy.emplace_back(s);
    }

    HashTable::HashTable<std::string, std::string_view> m;
    for (size_t i = 0; i < v.size(); i++)
    {
        const std::string &cstr = v[i];
        std::string &str = v_copy[i];
        m.emplace(std::move(str), cstr);
    }

    std::string_view nul = "NULL";
    size_t i = 0;
    for (const std::string &s : v)
    {
        const auto val = *m.find(s).value_or(&nul);
        std::cout << i++ << " - " << s << ": " << val << std::endl;
        assert(s == val);
    }

    for (const std::string &s : v_copy)
    {
        assert(s.empty());
    }
}