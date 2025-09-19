#pragma once

#include <java/util/Map.h>

class SwissTableHashMap;

/**
 * @brief Java internal unmodifiable map according to spec. Implemented under the hood as a swiss table.
 */
class UnmodifiableMap : public virtual Object, public virtual java::util::Map {
public:
    UnmodifiableMap();

    explicit UnmodifiableMap(int defaultSize);

    explicit UnmodifiableMap(const shared<Map>& map);

    static shared<UnmodifiableMap> copyOf(const shared<Map>& map);

protected:
    std::shared_ptr<Object> clone() override;

public:
    void lateinit() override;

    int hashCode() override;

    shared<String> toString() override;

    bool equals(Object *obj) override;

    DEFINE_SHARED_EQUALS

    void clear() override;

    shared<Object> compute(shared<Object> key, shared<java::util::function::BiFunction> remappingFunction) override;

    shared<Object> computeIfAbsent(shared<Object> key, shared<java::util::function::Function> mappingFunction) override;

    shared<Object>
    computeIfPresent(shared<Object> key, shared<java::util::function::BiFunction> remappingFunction) override;

    bool containsKey(shared<Object> key) override;

    bool containsValue(shared<Object> value) override;

    shared<java::util::Set> entrySet() override;

    void forEach(shared<java::util::function::BiConsumer> action) override;

    shared<Object> get(shared<Object> key) override;

    shared<Object> getOrDefault(shared<Object> key, shared<Object> defaultValue) override;

    bool isEmpty() override;

    shared<java::util::Set> keySet() override;

    shared<Object> merge(shared<Object> key, shared<Object> value,
        shared<java::util::function::BiFunction> remappingFunction) override;

    shared<Object> put(shared<Object> key, shared<Object> value) override;

    void putAll(shared<Map> m) override;

    shared<Object> putIfAbsent(shared<Object> key, shared<Object> value) override;

    shared<Object> remove(shared<Object> key) override;

    bool remove(shared<Object> key, shared<Object> value) override;

    shared<Object> replace(shared<Object> key, shared<Object> value) override;

    bool replace(shared<Object> key, shared<Object> oldValue, shared<Object> newValue) override;

    void replaceAll(shared<java::util::function::BiFunction> function) override;

    int size() override;

    /**
     * @brief This specific hash set has a linear complexity based on the length of the hashtable itself.
     *
     * e.g. The values collection is built using a linear scan of the table.
     * @return
     */
    shared<java::util::Collection> values() override;

    ~UnmodifiableMap() override;

private:
    SwissTableHashMap* table{};
};