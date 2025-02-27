#include <cstdint>
#include <optional>
#include <utility>
#include <variant>

// Open Address Hash Table
constexpr size_t HASH_TABLE_INIT_SIZE = 64;
constexpr float HASH_TABLE_GROW_FACTOR = 1.5;
constexpr float HASH_TABLE_MAX_LOAD_FACTOR = 0.5;
template <typename K, typename V>
class HashTable
{
private:
    using EmptySlot = struct _EmptySlot
    {
    };
    using DeletedSlot = struct _DeletedSlot
    {
    };
    using UsedSlot = std::pair<K, V>;
    using Slot = std::variant<EmptySlot, DeletedSlot, UsedSlot>;

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
                while (!std::holds_alternative<UsedSlot>(*m_cur) && m_cur < m_end)
                    m_cur += 1;
            }

        public:
            constexpr Inner(Slot *c, Slot *e) noexcept : m_cur(c), m_end(e)
            {
                while (!std::holds_alternative<UsedSlot>(*m_cur) && m_cur < m_end)
                    m_cur += 1;
            }
            constexpr UsedSlot &operator*() const noexcept
            {
                assert(std::holds_alternative<UsedSlot>(*m_cur));
                return std::get<UsedSlot>(*m_cur);
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
            std::fill(m_table.get(), m_table.get() + m_size, EmptySlot());
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

    // Returns a position of a slot that either matches the key or a usable free slot
    [[nodiscard]] inline std::optional<Slot *> find_slot(const K &key) noexcept
    {
        const size_t org_ipos = m_hasher(key) % capacity();
        size_t ipos = org_ipos;

        // Optional Deleted Slot
        std::optional<Slot *> first_del_slot = std::nullopt;

        // Linear Probe
        while (true)
        {
            // Define slot
            Slot &s = m_table[ipos];

            // Set first deleted slot if it is null
            if (!first_del_slot && std::holds_alternative<DeletedSlot>(s))
                first_del_slot.emplace(&s);

            // Return if key is the same
            if (std::holds_alternative<UsedSlot>(s) && std::get<UsedSlot>(s).first == key)
                return &s;

            // Return if slot is empty
            if (std::holds_alternative<EmptySlot>(s))
                return first_del_slot ? first_del_slot : &s; // Reuse deleted slot if found

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
        for (auto &[k, v] : other_table.key_values())
        {
            auto i = find_slot(k);
            assert(i.has_value());
            *i.value() = std::move(UsedSlot(std::move(k), std::move(v)));
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
        auto i = find_slot(key);
        assert(i.has_value());
        *i.value() = std::move(UsedSlot(std::forward<KK>(key), std::forward<VV>(val)));
        m_size += 1;
    }

    std::optional<V *> find(const K &key) noexcept
    {
        auto i = find_slot(key);
        if (!i.has_value() || std::holds_alternative<EmptySlot>(*i.value()) || std::holds_alternative<DeletedSlot>(*i.value()))
            return std::nullopt;
        else
            return static_cast<V *>(&(std::get<UsedSlot>(*i.value()).second));
    }

    // TODO: Remove function
};