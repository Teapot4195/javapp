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

        deferable(const deferable &other) = delete;

        deferable(deferable &&other) noexcept
            : func(std::move(other.func)) {
        }

        deferable & operator=(const deferable &other) = delete;

        deferable & operator=(deferable &&other) noexcept {
            if (this == &other)
                return *this;
            func = std::move(other.func);
            return *this;
        }
    };

    struct lateinit_frame {
        virtual ~lateinit_frame() = default;
    };

    struct lateinit_ll {
        lateinit_ll* last{nullptr};
        lateinit_frame* frame{nullptr};
        lateinit_ll* next{nullptr};
    };

    template <typename T>
    struct scoped_deferable {
        T* data;

        explicit scoped_deferable(T* data) : data(data) {}

        ~scoped_deferable() {
            delete data;
        }

        T* operator->() {
            return data;
        }

        T& operator*() {
            return *data;
        }

        scoped_deferable(const scoped_deferable &other) = delete;

        scoped_deferable(scoped_deferable &&other) noexcept
            : data(other.data) {
        }

        scoped_deferable & operator=(const scoped_deferable &other) = delete;

        scoped_deferable & operator=(scoped_deferable &&other) noexcept {
            if (this == &other)
                return *this;
            data = other.data;
            return *this;
        }
    };
}

struct lateinit_stack {
    internals::lateinit_ll* this_frame{nullptr};

    ~lateinit_stack();

    template <typename T, typename... Args>
    T* put(Args&&... args) {
        if (this_frame == nullptr) {
            this_frame = new internals::lateinit_ll;
        } else if (this_frame->next != nullptr) {
            // the next frame was allocated the last time around
            // advance the list and call it good
            this_frame = this_frame->next;
        } else {
            // we don't have any more stack frames left available
            this_frame->next = new internals::lateinit_ll;
            this_frame->next->last = this_frame;
            this_frame = this_frame->next;
        }
        T *allocd = new T(std::forward<Args>(args)...);
        this_frame->frame = allocd;

        return allocd;
    }

private:
    void die(std::string type);

public:
    template <typename T>
    internals::scoped_deferable<T> pop() {
        this_frame = this_frame->last;

        auto* ptr = dynamic_cast<T*>(this_frame->next->frame);
        if (ptr == nullptr) {
            die(typeid(T).name());
        }
        auto res = internals::scoped_deferable<T>{ptr}; // this is owning, we can set the frame to nullptr safely now

        this_frame->next->frame = nullptr;

        return std::move(res);
    }
};

extern lateinit_stack _G_stack;

class Object : public std::enable_shared_from_this<Object> {
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

    virtual void lateinit();

    void notify();

    void notifyAll();

    void wait();

    void wait(long long timeoutMillis);

    void wait(long long timeoutMillis, int nanos);

    virtual int hashCode();

    virtual std::shared_ptr<String> toString();

    virtual bool equals(Object* obj);

    /**
     * @brief THIS IS NOT DESIGNED TO BE HIDDEN, DO NOT HIDE
     * @param obj the object to compare
     */
    template <class T>
    requires std::derived_from<T, Object>
    bool equals(std::shared_ptr<T> obj) {
        return this->equals(obj.get());
    }

    /**
     * @brief THIS IS NOT DESIGNED TO BE HIDDEN, DO NOT HIDE
     */
    template <class T>
    requires std::derived_from<T, Object>
    bool instanceof() {
        return dynamic_cast<T*>(this) != nullptr;
    }

    internals::deferable synchronize();
};

template <typename T>
using shared = std::shared_ptr<T>;

template <typename T, typename... Args>
std::shared_ptr<T> alloc(Args&&... args) {
    auto shared = std::make_shared<T>(std::forward<Args>(args)...);
    if constexpr (std::derived_from<T, Object>) {
        shared->lateinit();
    }
    return shared;
}

#define DEFINE_SHARED_EQUALS \
template <class T> \
requires std::derived_from<T, Object> \
bool equals(std::shared_ptr<T> obj) { \
    return this->equals(obj.get()); \
}

template <typename T>
std::vector<shared<Object>> decay_vec(const std::vector<shared<T>>& source) {
    std::vector<shared<Object>> result;

    for (const auto& element : source)
        result.push_back(element);

    return result;
}
