#include "UnmodifiableMap.h"

#include <String.h>
#include <java/util/ArrayList.h>
#include <java/util/HashSet.h>
#include <java/util/Iterator.h>
#include <java/util/Objects.h>
#include <java/util/Set.h>
#include <java/util/function/BiConsumer.h>

#include "SwissTableHashMap.h"

UnmodifiableMap::UnmodifiableMap() {
    table = new SwissTableHashMap();
}

UnmodifiableMap::UnmodifiableMap(int defaultSize) {
    table = new SwissTableHashMap(defaultSize);
}

UnmodifiableMap::UnmodifiableMap(const shared<Map> &map) {
    table = new SwissTableHashMap(map);
}

shared<UnmodifiableMap> UnmodifiableMap::copyOf(const shared<Map>& map) {
    return std::make_shared<UnmodifiableMap>(map);
}

std::shared_ptr<Object> UnmodifiableMap::clone() {
    return shared_from_this();
}

void UnmodifiableMap::lateinit() {
    Map::lateinit();
}

int UnmodifiableMap::hashCode() {
    int hash_code = 1;
    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone) {
            hash_code = 31 * hash_code + table->table[i].key->hashCode();
            hash_code = 31 * hash_code + table->table[i].data->hashCode();
        }
    }

    return hash_code;
}

shared<String> UnmodifiableMap::toString() {
    std::string tostr = "{\n";

    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone) {
            tostr += "  {";
            tostr += table->table[i].key->toString()->get_backing();
            tostr += ",";
            tostr += table->table[i].data->toString()->get_backing();
            tostr += "},\n";
        }
    }

    tostr += "}";
    return alloc<String>(tostr);
}

bool UnmodifiableMap::equals(Object *obj) {
    if (!obj->instanceof<Map>())
        return false;
    if (this == obj)
        return true;
    Map* other = dynamic_cast<Map*>(obj);
    return other->entrySet()->equals(entrySet());
}

void UnmodifiableMap::clear() {
    table->table_clear();
}

shared<Object> UnmodifiableMap::
compute(shared<Object> key, shared<java::util::function::BiFunction> remappingFunction) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::computeIfAbsent(shared<Object> key,
                                                shared<java::util::function::Function> mappingFunction) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::computeIfPresent(shared<Object> key,
                                                 shared<java::util::function::BiFunction> remappingFunction) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

bool UnmodifiableMap::containsKey(const shared<Object> key) {
    auto [has, _] = table->table_get(key);
    return has;
}

bool UnmodifiableMap::containsValue(shared<Object> value) {
    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone) {
            if (java::util::Objects::equals(table->table[i].data, value))
                return true;
        }
    }

    return false;
}

class UnmodifiableMap_Entry_Specialization final : public virtual Object, public virtual java::util::Map::Entry {
    shared<Object> key;
    shared<Object> data;

public:
    UnmodifiableMap_Entry_Specialization(const shared<Object> &key, const shared<Object> &data) : key(key), data(data) {}

    shared<Object> getKey() override {
        return key;
    }

    shared<Object> getValue() override {
        return data;
    }

    shared<Object> setValue(shared<Object> value) override {
        throw std::runtime_error("THROW UNSUPPOREDOPERATIONEXCEPTION!");
    }
};

shared<java::util::Set> UnmodifiableMap::entrySet() {
    shared<java::util::Set> collection = alloc<java::util::HashSet>(table->table_size);

    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone)
            collection->add(alloc<UnmodifiableMap_Entry_Specialization>(table->table[i].key, table->table[i].data));
    }

    return collection;
}

void UnmodifiableMap::forEach(shared<java::util::function::BiConsumer> action) {
    for (int i = 0; i < table->table_size; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone)
            action->accept(table->table[i].key, table->table[i].data);
    }
}

shared<Object> UnmodifiableMap::get(shared<Object> key) {
    if (auto [has, obj] = table->table_get(key); has)
        return obj;
    return null;
}

shared<Object> UnmodifiableMap::getOrDefault(shared<Object> key, shared<Object> defaultValue) {
    if (auto [has, obj] = table->table_get(key); has)
        return obj;
    return defaultValue;
}

bool UnmodifiableMap::isEmpty() {
    return table->table_size == 0;
}

shared<java::util::Set> UnmodifiableMap::keySet() {
    shared<java::util::Set> collection = alloc<java::util::HashSet>(table->table_size);

    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone)
            collection->add(table->table[i].data);
    }

    return collection;
}

shared<Object> UnmodifiableMap::merge(shared<Object> key, shared<Object> value,
                                      shared<java::util::function::BiFunction> remappingFunction) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::put(shared<Object> key, shared<Object> value) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

void UnmodifiableMap::putAll(shared<Map> m) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::putIfAbsent(shared<Object> key, shared<Object> value) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::remove(shared<Object> key) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

bool UnmodifiableMap::remove(shared<Object> key, shared<Object> value) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

shared<Object> UnmodifiableMap::replace(shared<Object> key, shared<Object> value) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

bool UnmodifiableMap::replace(shared<Object> key, shared<Object> oldValue, shared<Object> newValue) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

void UnmodifiableMap::replaceAll(shared<java::util::function::BiFunction> function) {
    throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
}

int UnmodifiableMap::size() {
    return table->table_size;
}

shared<java::util::Collection> UnmodifiableMap::values() {
    shared<java::util::Collection> collection = alloc<java::util::ArrayList>(table->table_size);

    for (int i = 0; i < table->bucket_count; i++) {
        if (table->metadata[i].usage && !table->metadata[i].tombstone)
            collection->add(table->table[i].data);
    }

    return collection;
}

UnmodifiableMap::~UnmodifiableMap() {
    delete table;
}
