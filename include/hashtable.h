#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>
#include <utility>
#include <variant>

namespace HashTable
{
    // Open Address Hash Table
    constexpr size_t HASH_TABLE_INIT_SIZE = 2;
    constexpr float HASH_TABLE_GROW_FACTOR = 2;
    constexpr float HASH_TABLE_MAX_LOAD_FACTOR = 0.7;
    template <typename K, typename V>
    class HashTable
    {
    private:
        class Slot
        {
        public:
            enum Type : uint8_t
            {
                Deleted,
                Empty,
                Used
            };

            // ctors
            constexpr Slot() noexcept : m_type(Type::Empty) {}

            // member functions
            [[nodiscard]] constexpr Type type() const noexcept { return m_type; }
            [[nodiscard]] constexpr bool used() const noexcept { return m_type == Type::Used; }
            [[nodiscard]] constexpr bool empty() const noexcept { return m_type == Type::Empty; }
            [[nodiscard]] constexpr bool deleted() const noexcept { return m_type == Type::Deleted; }
            [[nodiscard]] constexpr const K &ckey() const noexcept
            {
                assert(used());
                return m_key;
            }
            [[nodiscard]] constexpr size_t hash() const noexcept
            {
                assert(used());
                return m_hash;
            }
            [[nodiscard]] constexpr K &key() noexcept
            {
                assert(used());
                return m_key;
            }
            [[nodiscard]] constexpr V &val() noexcept
            {
                assert(used());
                return m_val;
            }
            template <typename KK, typename VV>
            constexpr std::optional<std::pair<K, V>> emplace(size_t hash, KK &&k, VV &&v) noexcept
            {
                std::optional<std::pair<K, V>> retval = used() ? std::optional(std::pair<K, V>(std::move(m_key), std::move(m_val))) : std::nullopt;
                m_type = Type::Used;
                m_key = std::forward<KK>(k);
                m_val = std::forward<VV>(v);
                m_hash = hash;
                return retval;
            }

        private:
            size_t m_hash;
            Type m_type;
            K m_key;
            V m_val;
        };

        class KVIter
        {
        private:
            class Inner
            {
            private:
                Slot *m_cur;
                Slot *m_end;

                constexpr void next() noexcept
                {
                    m_cur += 1;
                    while (!m_cur->used() && m_cur < m_end)
                        m_cur += 1;
                }

            public:
                constexpr Inner(Slot *c, Slot *e) noexcept : m_cur(c), m_end(e)
                {
                    while (!m_cur->used() && m_cur < m_end)
                        m_cur += 1;
                }
                constexpr Slot &operator*() const noexcept
                {
                    assert(m_cur->used());
                    return *m_cur;
                }
                constexpr bool operator==(const Inner &rhs) const noexcept { return m_cur == rhs.m_cur; }
                constexpr bool operator!=(const Inner &rhs) const noexcept { return !(m_cur == rhs.m_cur); }
                constexpr Inner &operator++() noexcept
                {
                    next();
                    return *this;
                }
                constexpr Inner operator++(int) noexcept
                {
                    auto retval = Inner(m_cur, m_end);
                    next();
                    return retval;
                }
            };

            Slot *m_ptr;
            size_t m_size;

        public:
            constexpr KVIter(Slot *p, size_t s) noexcept : m_ptr(p), m_size(s) {}
            [[nodiscard]] constexpr Inner begin() const noexcept { return Inner(m_ptr, m_ptr + m_size); }
            [[nodiscard]] constexpr Inner end() const noexcept { return Inner(m_ptr + m_size, m_ptr + m_size); }
        };

        // InnerTable
        class InnerTable
        {
        private:
            std::unique_ptr<Slot[]> m_table;
            size_t m_size;

        public:
            constexpr InnerTable(size_t s) noexcept : m_table(std::make_unique<Slot[]>(s)), m_size(s)
            {
                std::fill(m_table.get(), m_table.get() + m_size, Slot());
            }

            [[nodiscard]] constexpr size_t size() const noexcept
            {
                return m_size;
            }

            [[nodiscard]] constexpr Slot &operator[](size_t i) noexcept
            {
                assert(i < m_size);
                return m_table[i];
            }

            [[nodiscard]] constexpr const Slot &operator[](size_t i) const noexcept
            {
                assert(i < m_size);
                return m_table[i];
            }

            [[nodiscard]] constexpr KVIter key_values() noexcept
            {
                return KVIter(m_table.get(), m_size);
            }

            [[nodiscard]] constexpr Slot *data() noexcept
            {
                return m_table.get();
            }
        };

        // Member variables
        InnerTable m_table;
        size_t m_size;
        std::hash<K> m_hasher;

        [[nodiscard]] static constexpr float load_factor(size_t size, size_t cap) noexcept { return static_cast<float>(size) / static_cast<float>(cap); }

        // Returns a position of a slot that either matches the key or a usable slot
        [[nodiscard]] inline std::optional<Slot *> find_slot(const size_t hash, const K &key) noexcept
        {
            const size_t org_ipos = hash % capacity();
            size_t ipos = org_ipos;

            // Optional Deleted Slot
            std::optional<Slot *> first_del_slot = std::nullopt;

            // Linear Probe
            while (true)
            {
                // Define slot
                Slot &s = m_table[ipos];

                switch (s.type())
                {
                case Slot::Type::Empty:                          // Return if slot is empty
                    return first_del_slot ? first_del_slot : &s; // Reuse deleted slot if found
                case Slot::Type::Deleted:                        // Set first deleted slot if it is null
                    if (!first_del_slot)
                        first_del_slot.emplace(&s);
                    break;
                case Slot::Type::Used: // Return if key is the same
                    if (s.hash() == hash && s.ckey() == key)
                        return &s;
                    break;
                default:
                    __builtin_unreachable();
                }

                // Increment cursor
                ipos = (ipos + 1) % capacity();

                // Return nullopt if table is full
                if (ipos == org_ipos)
                    return std::nullopt;
            }
        }

        void rehash(size_t new_cap) noexcept
        {
            // Make new table
            InnerTable other_table = InnerTable(new_cap);

            // Swap table
            std::swap(m_table, other_table);

            // Iterate old table and insert to new table
            size_t new_size = 0;
            for (Slot &other_s : other_table.key_values())
            {
                auto new_s = find_slot(other_s.hash(), other_s.ckey());
                assert(new_s.has_value());
                new_s.value()->emplace(other_s.hash(), std::move(other_s.key()), std::move(other_s.val()));
                new_size += 1;
            }
            m_size = new_size;
        }

    public:
        // ctors
        HashTable() noexcept : m_table(HASH_TABLE_INIT_SIZE), m_size(0) {}

        // getters
        [[nodiscard]] constexpr size_t capacity() const noexcept { return m_table.size(); }
        [[nodiscard]] constexpr size_t size() const noexcept { return m_size; }

        // functions
        template <typename KK, typename VV>
        void emplace(KK &&key, VV &&val) noexcept
        {
            // Rehash if over load factor limit
            if (load_factor(m_size + 1, capacity()) >= HASH_TABLE_MAX_LOAD_FACTOR)
            {
                size_t new_cap = static_cast<size_t>(static_cast<float>(capacity()) * HASH_TABLE_GROW_FACTOR);
                rehash(new_cap);
            }

            // Hash & insert
            const size_t hash = m_hasher(key);
            auto i = find_slot(hash, key);
            assert(i.has_value());
            i.value()->emplace(hash, std::forward<KK>(key), std::forward<VV>(val));
            m_size += 1;
        }

        std::optional<V *> find(const K &key) noexcept
        {
            const size_t hash = m_hasher(key);
            std::optional<Slot *> os = find_slot(hash, key);
            if (!os)
                return std::nullopt;
            Slot *s = os.value();
            if (s->empty() || s->deleted())
                return std::nullopt;
            else
                return static_cast<V *>(&s->val());
        }

        // TODO: Remove function
    };
}