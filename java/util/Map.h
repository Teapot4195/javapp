#pragma once

#include <Object.h>

namespace java::util {
    class Collection;
}

namespace java::util::function {
    class BiConsumer;
}

namespace java::util {
    class Set;
}

namespace java::util {
    namespace function {
        class BiFunction;
        class Function;
    }

    class Comparator;

    class Map : public virtual Object {
    public:
        class Entry : public virtual Object {
        public:
            ~Entry() override = default;

            static shared<Comparator> comparingByKey();

            static shared<Comparator> comparingByKey(const shared<Comparator>& comparator);

            static shared<Comparator> comparingByValue();

            static shared<Comparator> comparingByValue(shared<Comparator> comparator);

            static shared<Entry> copyOf(shared<Entry> e);

            virtual shared<Object> getKey() = 0;

            virtual shared<Object> getValue() = 0;

            virtual shared<Object> setValue(shared<Object> value) = 0;
        };

        virtual void clear() = 0;

        virtual shared<Object> compute(shared<Object> key, shared<function::BiFunction> remappingFunction);

        virtual shared<Object> computeIfAbsent(shared<Object> key, shared<function::Function> mappingFunction);

        virtual shared<Object> computeIfPresent(shared<Object> key, shared<function::BiFunction> remappingFunction);

        virtual bool containsKey(shared<Object> key) = 0;

        virtual bool containsValue(shared<Object> value) = 0;

        static shared<Map> copyOf(shared<Map> map);

        static shared<Entry> entry(const shared<Object>& k, const shared<Object>& v);

        virtual shared<Set> entrySet() = 0;

        virtual void forEach(shared<function::BiConsumer> action);

        virtual shared<Object> get(shared<Object> key) = 0;

        virtual shared<Object> getOrDefault(shared<Object> key, shared<Object> defaultValue);

        virtual bool isEmpty() = 0;

        virtual shared<Set> keySet() = 0;

        virtual shared<Object> merge(shared<Object> key, shared<Object> value, shared<function::BiFunction> remappingFunction);

        static shared<Map> of(const std::vector<shared<Object>> &data);

        template<typename... Args>
        static shared<Map> of(Args&&... args) {
            return of({std::forward<Args...>(args)...});
        }

        static shared<Map> ofEntries(const std::vector<shared<Entry>>& entries);

        virtual shared<Object> put(shared<Object> key, shared<Object> value) = 0;

        virtual void putAll(shared<Map> m) = 0;

        virtual shared<Object> putIfAbsent(shared<Object> key, shared<Object> value);

        virtual shared<Object> remove(shared<Object> key) = 0;

        virtual bool remove(shared<Object> key, shared<Object> value);

        virtual shared<Object> replace(shared<Object> key, shared<Object> value);

        virtual bool replace(shared<Object> key, shared<Object> oldValue, shared<Object> newValue);

        virtual void replaceAll(shared<function::BiFunction> function);

        virtual int size() = 0;

        virtual shared<Collection> values() = 0;
    };
}
