#include "Map.h"

#include <utility>

#include <impl_inner/UnmodifiableMap.h>
#include <java/util/Map.h>

#include "Comparable.h"
#include "Comparator.h"
#include "Iterator.h"
#include "Objects.h"
#include "Set.h"
#include "function/BiConsumer.h"
#include "function/BiFunction.h"
#include "function/Function.h"

namespace java::util {
    shared<Comparator> Map::Entry::comparingByKey() {
        return alloc<Comparator>([](const shared<Object>& o1, const shared<Object>& o2) {
            const auto obj1 = std::dynamic_pointer_cast<Entry>(o1);
            const auto obj2 = std::dynamic_pointer_cast<Entry>(o2);
            const auto obj1k = std::dynamic_pointer_cast<Comparable>(obj1->getKey());
            const auto obj2k = std::dynamic_pointer_cast<Comparable>(obj2->getKey());
            return obj1k->compareTo(obj2k);
        });
    }

    shared<Comparator> Map::Entry::comparingByKey(const shared<Comparator>& comparator) {
        return alloc<Comparator>([comparator](const shared<Object>& o1, const shared<Object>& o2) {
            const auto obj1 = std::dynamic_pointer_cast<Entry>(o1);
            const auto obj2 = std::dynamic_pointer_cast<Entry>(o2);
            const auto obj1k = obj1->getKey();
            const auto obj2k = obj2->getKey();
            return comparator->compare(obj1k, obj2k);
        });
    }

    shared<Comparator> Map::Entry::comparingByValue() {
        return alloc<Comparator>([](const shared<Object>& o1, const shared<Object>& o2) {
            const auto obj1 = std::dynamic_pointer_cast<Entry>(o1);
            const auto obj2 = std::dynamic_pointer_cast<Entry>(o2);
            const auto obj1k = std::dynamic_pointer_cast<Comparable>(obj1->getValue());
            const auto obj2k = std::dynamic_pointer_cast<Comparable>(obj2->getValue());
            return obj1k->compareTo(obj2k);
        });
    }

    shared<Comparator> Map::Entry::comparingByValue(shared<Comparator> comparator) {
        return alloc<Comparator>([comparator](const shared<Object>& o1, const shared<Object>& o2) {
            const auto obj1 = std::dynamic_pointer_cast<Entry>(o1);
            const auto obj2 = std::dynamic_pointer_cast<Entry>(o2);
            const auto obj1k = obj1->getValue();
            const auto obj2k = obj2->getValue();
            return comparator->compare(obj1k, obj2k);
        });
    }

    class MapEntryCopyOfSpecialization final : public virtual Object, public virtual Map::Entry {
        shared<Object> key;
        shared<Object> value;
    public:
        MapEntryCopyOfSpecialization(shared<Object> key, shared<Object> value);

        shared<Object> getKey() override {
            return key;
        }

        shared<Object> getValue() override {
            return value;
        }

        shared<Object> setValue(shared<Object> value) override {
            throw std::runtime_error("throw UNSUPPORTEDOPERATIONEXCEPTION!");
        }
    };

    shared<Map::Entry> Map::Entry::copyOf(shared<Entry> e) {
        static auto tid = std::type_index(typeid(MapEntryCopyOfSpecialization));
        if (std::type_index(typeid(e)) == tid)
            return e;
        return alloc<MapEntryCopyOfSpecialization>(e->getKey(), e->getValue());
    }

    shared<Object> Map::compute(const shared<Object> key, const shared<function::BiFunction> remappingFunction) {
        auto oldValue = get(key);
        auto newValue = remappingFunction->apply(key, oldValue);
        if (newValue != null)
            put(key, newValue);
        else if (oldValue != null || containsKey(key))
            remove(key);
        return newValue;
    }

    shared<Object> Map::computeIfAbsent(const shared<Object> key, const shared<function::Function> mappingFunction) {
        auto cval = get(key);
        if (cval == null) {
            if (auto newValue = mappingFunction->apply(key); newValue != null) {
                put(key, newValue);
                return newValue;
            }
            return null;
        }
        return cval;
    }

    shared<Object> Map::computeIfPresent(shared<Object> key, shared<function::BiFunction> remappingFunction) {
        if (const auto cval = get(key); cval != null) {
            if (auto newValue = remappingFunction->apply(key, cval); newValue != null) {
                put(key, newValue);
                return newValue;
            }
            remove(key);
            return null;
        }
        return null;
    }

    shared<Map> Map::copyOf(shared<Map> map) {
        if (map->instanceof<UnmodifiableMap>())
            return map; // no copy.
        return UnmodifiableMap::copyOf(std::move(map));
    }

    class Map_Entry_Specialization final : public virtual Object, public virtual Map::Entry {
        shared<Object> key;
        shared<Object> value;

    public:
        Map_Entry_Specialization(const shared<Object> &key, const shared<Object> &value)
            : key(key),
              value(value) {
        }

        shared<Object> getKey() override {
            return key;
        }

        shared<Object> getValue() override {
            return value;
        }

        shared<Object> setValue(shared<Object> _) override {
            throw std::runtime_error("THROW UNSUPPORTEDOPERATIONEXCEPTION!");
        }
    };

    shared<Map::Entry> Map::entry(const shared<Object>& k, const shared<Object>& v) {
        if (!k || !v)
            throw std::runtime_error("THROW NULLPOINTEREXCEPTION!");

        return alloc<Map_Entry_Specialization>(k, v);
    }

    void Map::forEach(shared<function::BiConsumer> action) {
        FOREACH(Entry, entry, entrySet(), action->accept(entry->getKey(), entry->getValue());)
    }

    shared<Object> Map::getOrDefault(shared<Object> key, shared<Object> defaultValue) {
        auto v = get(std::move(key));
        if (v == nullptr)
            v = std::move(defaultValue);

        return v;
    }

    shared<Object> Map::merge(const shared<Object> key, const shared<Object> value,
        const shared<function::BiFunction> remappingFunction) {
        const auto oldValue = get(key);
        const auto newValue = oldValue == null ? value : remappingFunction->apply(oldValue, value);
        if (newValue == null)
            remove(key);
        else
            put(key, newValue);

        return newValue;
    }

    shared<Map> Map::of(const std::vector<shared<Object>> &data) {
        if (data.size() % 2 != 0)
            panic("Incorrect numbers of arguments (impossible case)");

        shared<UnmodifiableMap> map = alloc<UnmodifiableMap>(data.size() / 2);
        for (int i = 0; i < data.size() / 2; i += 2) {
            map->put(data[i], data[i + 1]);
        }

        return map;
    }

    shared<Map> Map::ofEntries(const std::vector<shared<Entry>>& entries) {
        shared<UnmodifiableMap> map = alloc<UnmodifiableMap>(entries.size());
        for (const auto & entry : entries) {
            map->put(entry->getKey(), entry->getValue());
        }

        return map;
    }

    shared<Object> Map::putIfAbsent(shared<Object> key, shared<Object> value) {
        auto v = get(key);
        if (v == nullptr)
            v = put(std::move(key), std::move(value));

        return v;
    }

    bool Map::remove(const shared<Object> key, const shared<Object> value) {
        if (containsKey(key) && Objects::equals(get(key), value)) {
            remove(key);
            return true;
        }
        return false;
    }

    shared<Object> Map::replace(const shared<Object> key, shared<Object> value) {
        if (containsKey(key))
            return put(key, std::move(value));
        return nullptr;
    }

    bool Map::replace(const shared<Object> key, const shared<Object> oldValue, shared<Object> newValue) {
        if (containsKey(key) && Objects::equals(get(key), oldValue)) {
            put(key, std::move(newValue));
            return true;
        }
        return false;
    }

    void Map::replaceAll(const shared<function::BiFunction> function) {
        FOREACH(Entry, entry, entrySet(), entry->setValue(function->apply(entry->getKey(), entry->getValue()));)
    }
}
