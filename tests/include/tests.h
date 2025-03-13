#include <cstdint>
#include <random>
#include <string>
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
        randomString.push_back(characters[distribution(generator)]);

    return randomString;
}

std::vector<std::string> make_rand_vec(size_t vec_size, size_t str_size)
{
    std::vector<std::string> v;
    v.reserve(vec_size);
    for (size_t i = 0; i < vec_size; i++)
    {
        while (true)
        {
            std::string str = generateRandomString(str_size);
            if (std::find(v.cbegin(), v.cend(), str) != v.cend())
                continue;
            v.push_back(std::move(str));
            break;
        }
    }
    return v;
}

std::vector<std::string> make_rand_vec(size_t vec_size, size_t str_size, const std::vector<std::string> &exclude)
{
    std::vector<std::string> v;
    v.reserve(vec_size);
    for (size_t i = 0; i < vec_size; i++)
    {
        while (true)
        {
            std::string str = generateRandomString(str_size);
            if (std::find(v.cbegin(), v.cend(), str) != v.cend())
                continue;
            if (std::find(exclude.cbegin(), exclude.cend(), str) != exclude.cend())
                continue;
            v.push_back(std::move(str));
            break;
        }
    }
    return v;
}

class NoCopyString : public std::string
{
public:
    NoCopyString(const std::string &s) noexcept : std::string(s) {}
    NoCopyString(const NoCopyString &) = delete;
    NoCopyString &operator=(const NoCopyString &) = delete;
    NoCopyString(NoCopyString &&) = default;
    NoCopyString &operator=(NoCopyString &&) = default;
};