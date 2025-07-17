#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

class String;

extern std::unordered_map<std::thread::id, std::stop_source> _G_stop_sources;
extern std::unordered_map<std::thread::id, std::stop_token> _G_stop_tokens;

namespace internals {
    struct deferable {
        std::function<void()> func;

        explicit deferable(const std::function<void()> &func) : func(func) {}

        ~deferable() {
            func();
        }
    };
}

class Object : std::enable_shared_from_this<Object> {
    struct typeData {
        std::size_t size;
    };
    static std::unordered_map<std::type_index, typeData> typeMap;

    std::recursive_mutex _monitor_mutex;
    std::condition_variable_any _monitor_cond;

public:
        virtual ~Object() = default;

protected:
    virtual std::shared_ptr<Object> clone();

public:
    template <typename T>
    static void registerType() {
        const auto id = std::type_index(typeid(T));
        typeMap[id] = {
            sizeof(T),
        };
    }

    void notify();

    void notifyAll();

    void wait();

    void wait(long long timeoutMillis);

    void wait(long long timeoutMillis, int nanos);

    virtual int hashCode();

    virtual std::shared_ptr<String> to_string();

    virtual bool equals(Object* obj);

    /**
     * @brief THIS IS NOT DESIGNED TO BE OVERRIDEN, DO NOT OVERRIDE
     * @param obj the object to compare
     */
    template <class T>
    requires std::derived_from<T, Object>
    bool equals(std::shared_ptr<T> obj) {
        return this->equals(obj.get());
    }

    internals::deferable synchronize();
};

template <typename T>
using shared = std::shared_ptr<T>;

template <typename T, typename... Args>
 std::shared_ptr<T> alloc(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
