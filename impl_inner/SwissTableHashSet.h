#pragma once

#include <Object.h>

#include "HashTableCommon.h"

namespace java::util {
    class Map;
}

class SwissTableHashSet {
public:
    SwissTableHashSet();

    explicit SwissTableHashSet(const shared<java::util::Map> &map);

    bool table_delete(const shared<Object> &key);

    bool table_insert_or_set(const shared<Object> &key);

    bool table_get(const shared<Object> &key) const;

    [[nodiscard]] constexpr float load_factor() const;

    [[nodiscard]] constexpr float tombstone_factor() const;

    void rebuild_table(size_t new_bucket_count);

    void table_clear();

    /**
     * @brief Rebuild this hash table, removing the deleted entries.
     */
    void rebuild_table();

#pragma pack(1)
    struct swisstable {
        shared<Object> key;
        uint hash;
    };
    struct metadata {
        bool usage : 1;
        bool tombstone : 1;
        uint8_t hash_low : 6;
    };
#pragma pop(1)

    size_t bucket_count{};
    size_t tombstone_count{};
    size_t table_size{};

    swisstable* table {nullptr};
    metadata* metadata {nullptr};

    static bool insert_table(swisstable* table, struct metadata* meta, size_t bucket_count, uint hash,
        const shared<Object> &key);

    static bool get_table(swisstable* table, const struct metadata* meta, size_t bucket_count, uint hash,
        const shared<Object> &key);

    static bool delete_table(swisstable* table, struct metadata* meta, size_t bucket_count, uint hash, const shared<Object> &key);
};

constexpr float SwissTableHashSet::load_factor() const {
    return static_cast<float>(table_size) / static_cast<float>(bucket_count);
}

constexpr float SwissTableHashSet::tombstone_factor() const {
    return static_cast<float>(tombstone_count) / static_cast<float>(bucket_count);
}
