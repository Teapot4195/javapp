#include "SwissTableHashMap.h"

#include <java/util/Iterator.h>
#include <java/util/Map.h>
#include <java/util/Set.h>

SwissTableHashMap::SwissTableHashMap() {
    table = new swisstable[default_bucket_count];
    metadata = new struct metadata[default_bucket_count];
    bucket_count = default_bucket_count;
}

SwissTableHashMap::SwissTableHashMap(const shared<java::util::Map> &map) {
    table = new swisstable[default_bucket_count];
    metadata = new struct metadata[default_bucket_count];
    bucket_count = default_bucket_count;
    FOREACH(java::util::Map::Entry, x, map->entrySet(), {
        auto key = x->getKey();
        auto value = x->getValue();
        if (!key || !value)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION!");
        if (!table_insert_or_set(key, value))
            throw std::runtime_error("THROW ARGUMENTEXCEPTION!");
    });
}

std::pair<bool, shared<Object>> SwissTableHashMap::table_delete(const shared<Object> &key) {
    if (load_factor() > load_factor_limit || tombstone_factor() > tombstone_factor_limit)
        rebuild_table();
    return delete_table(table, metadata, bucket_count, key->hashCode(), key);
}

bool SwissTableHashMap::table_insert_or_set(const shared<Object> &key, const shared<Object> &value) {
    if (load_factor() > load_factor_limit || tombstone_factor() > tombstone_factor_limit)
        rebuild_table();
    return insert_table(table, metadata, bucket_count, key->hashCode(), key, value);
}

std::pair<bool, shared<Object>> SwissTableHashMap::table_get(const shared<Object> &key) const {
    return get_table(table, metadata, bucket_count, key->hashCode(), key);
}

void SwissTableHashMap::rebuild_table(size_t new_bucket_count) {
    swisstable* newTable = new swisstable[new_bucket_count];
    struct metadata* newMetadata = new struct metadata[new_bucket_count];

    size_t real_count = 0;

    for (int i = 0; i < bucket_count; i++) {
        if (metadata[i].usage && !metadata[i].tombstone) {
            real_count++;
            insert_table(newTable, newMetadata, new_bucket_count, table[i].hash, table[i].key, table[i].data);
        }
    }

    delete[] table;
    delete[] metadata;

    bucket_count = new_bucket_count;
    tombstone_count = real_count;
    table_size = real_count;

    table = newTable;
    metadata = newMetadata;
}

void SwissTableHashMap::table_clear() {
    for (int i = 0; i < bucket_count; i++) {
        table[i].data.reset();
        table[i].key.reset();
        metadata[i].usage = false;
        metadata[i].tombstone = false;
    }

    table_size = 0;
    tombstone_count = 0;
}

void SwissTableHashMap::rebuild_table() {
    rebuild_table(bucket_count);
}

std::pair<bool, shared<Object>> SwissTableHashMap::delete_table(swisstable *table, struct metadata *meta, size_t bucket_count,
    uint hash, const shared<Object> &key) {
    // compute the entry that would usually go into.
    size_t expected_index = hash % bucket_count;
    size_t orig_expected_index = expected_index;

    if (!meta[expected_index].usage) {
        meta[expected_index].tombstone = true;
        return {false, null};
    }

    while (true) {
        if (meta[expected_index].usage && !meta[expected_index].tombstone && table[expected_index].hash == hash &&
            table[expected_index].key == key && table[expected_index].key->equals(key)) {
            meta[expected_index].tombstone = true;
            auto moved = std::move(table[expected_index].data);
            table[expected_index].data.reset();
            table[expected_index].key.reset();
            return {true, moved};
        }

        if (!meta[expected_index].usage)
            break;

        expected_index++;
        if (orig_expected_index == expected_index)
            panic("Wrapped around entire table but no matching key!"
                " Should not wrap around entire table because of load factor!");

        if (expected_index == bucket_count)
            expected_index = 0;
    }

    return {false, null};
}

bool SwissTableHashMap::insert_table(swisstable *table, struct metadata *meta, size_t bucket_count, uint hash,
                                     const shared<Object> &key, const shared<Object> &value) {
    // compute the entry that it would usually go into.
    size_t expected_index = hash % bucket_count;
    size_t orig_expected_index = expected_index;
    int64_t insert_candidate_index = -1;

    if (!meta[expected_index].usage) {
        table[expected_index] = {key, value, hash};
        meta[expected_index] = {true, false, static_cast<uint8_t>(hash & 0x3F)};
        return true;
    }

    while (true) {
        if (insert_candidate_index == -1 && meta[expected_index].tombstone)
            insert_candidate_index = expected_index;

        if (meta[expected_index].usage && table[expected_index].hash == hash && table[expected_index].key == key &&
            table[expected_index].key->equals(key)) {
            table[expected_index].data = value;
            return false;
        }

        if (!meta[expected_index].usage)
            break;

        expected_index++;
        if (orig_expected_index == expected_index && insert_candidate_index == -1)
            panic("Wrapped around entire table but no candidate insertion index or matching key!");

        if (expected_index == bucket_count)
            expected_index = 0;
    }

    if (insert_candidate_index != -1)
        expected_index = insert_candidate_index;

    table[expected_index] = {key, value, hash};
    meta[expected_index] = {true, false, static_cast<uint8_t>(hash & 0x3F)};
    return true;
}

std::pair<bool, shared<Object>> SwissTableHashMap::get_table(swisstable *table, const struct metadata *meta, const size_t bucket_count,
    const uint hash, const shared<Object> &key) {
    // compute the entry that it would usually go into.
    size_t expected_index = hash % bucket_count;
    size_t orig_expected_index = expected_index;

    if (!meta[expected_index].usage)
        return {false, null};

    while (true) {
        if (meta[expected_index].usage && table[expected_index].hash == hash && table[expected_index].key == key &&
            table[expected_index].key->equals(key)) {
            return {true, table[expected_index].data};
        }

        if (!meta[expected_index].usage)
            break;

        expected_index++;
        if (orig_expected_index == expected_index)
            panic("Wrapped around entire table but no matching key!"
                " Should not wrap around entire table because of load factor!");

        if (expected_index == bucket_count)
            expected_index = 0;
    }

    return {false, null};
}
