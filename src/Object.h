#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>

#include <ucontext.h>

#include <AllocTrace.h>
#include <list>
#include <rttr/rttr_enable.h>

class Object;
class String;

struct thread_information {
    ucontext_t context;
};

extern std::unordered_map<std::thread::id, std::stop_source> _G_stop_sources;
extern std::unordered_map<std::thread::id, std::stop_token> _G_stop_tokens;
extern std::unordered_map<std::thread::id, thread_information> _G_stop_information;

extern std::mutex _gc_mutex;
extern std::size_t _G_thread_count; ///< thread count is also protected using _gc_mutex.
extern std::size_t _gc_at_barrier;
extern std::condition_variable _gc_cond;
extern std::condition_variable _gc_ok;
extern std::condition_variable _gc_stop;
extern bool _gc_done;

extern thread_local std::stop_token _thread_stop;

extern std::stop_source _program_stop;
extern std::stop_token _program_stop_token;

extern std::vector<std::thread> _G_threads;

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

    struct typeData {
        std::size_t size;
        std::string name;
        std::function<Object*(void*)> upcaster;
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

#ifdef JAVAPP_ENABLE_GC
template <typename T>
class shared {
    T* held_ptr;

public:
    explicit shared(T* ptr) : held_ptr(ptr) {}

    shared(std::nullptr_t _) : held_ptr(nullptr) {}

    shared() : held_ptr(nullptr) {}

    T& operator*() const {
        if (!held_ptr)
            throw std::runtime_error("Null Pointer Exception!");
        return held_ptr;
    }

    T* operator->() const {
        if (!held_ptr)
            throw std::runtime_error("Null Pointer Exception!");
        return held_ptr;
    }

    [[nodiscard]] T* get() const {
        return held_ptr;
    }

    void reset() {
        held_ptr = nullptr;
    }

    template <typename V>
    friend shared<V> dynamic_pointer_cast(const shared& src) {
        if (!src.held_ptr)
            return shared<V>{nullptr};
        auto* result = dynamic_cast<V*>(src.held_ptr);
        if (!result)
            throw std::runtime_error("Bad Cast Exception!");
        return shared<V>{result};
    }

    template <typename V>
    friend shared<V> static_pointer_cast(const shared& src) {
        if (!src.held_ptr)
            return shared<V>{nullptr};
        auto* result = static_cast<V*>(src.held_ptr);
        return shared<V>{result};
    }

    template <typename V>
    friend shared<V> const_pointer_cast(const shared& src) {
        if (!src.held_ptr)
            return shared<V>{nullptr};
        auto* result = const_cast<V*>(src.held_ptr);
        return shared<V>{result};
    }

    template <typename V>
    friend shared<V> reinterpret_pointer_cast(const shared& src) {
        if (!src.held_ptr)
            return shared<V>{nullptr};
        auto* result = reinterpret_cast<V*>(src.held_ptr);
        return shared<V>{result};
    }

    template <typename V>
    friend bool operator==(const shared &a, const shared<V>& b) {
        return a.held_ptr == b.held_ptr;
    }

    friend bool operator==(const shared &a, const std::nullptr_t _) {
        return a.held_ptr == nullptr;
    }

    template <typename V>
    requires std::derived_from<T, V>
    operator shared<V>() const {
        return static_pointer_cast<V>(*this);
    }

    operator bool() const {
        return held_ptr != nullptr;
    }

    friend bool operator<(const shared &lhs, const shared &rhs) {
        return lhs.held_ptr < rhs.held_ptr;
    }
};
#else
template <typename T>
using shared = std::shared_ptr<T>;

// template <typename V, typename T>
// shared<V> dynamic_pointer_cast(const shared<T>& src) {
//     if (!src)
//         return shared<V>{nullptr};
//     auto result = std::dynamic_pointer_cast<V>(src);
//     if (!result)
//         throw std::runtime_error("Bad Cast Exception!");
//     return result;
// }
//
// template <typename V, typename T>
// shared<V> static_pointer_cast(const shared<T>& src) {
//     if (!src)
//         return shared<V>{nullptr};
//     return std::static_pointer_cast<V>(src);
// }
//
// template <typename V, typename T>
// shared<V> const_pointer_cast(const shared<T>& src) {
//     if (!src)
//         return shared<V>{nullptr};
//     return std::const_pointer_cast<V>(src);
// }
//
// template <typename V, typename T>
// shared<V> reinterpret_pointer_cast(const shared<T>& src) {
//     if (!src)
//         return shared<V>{nullptr};
//     return std::reinterpret_pointer_cast<V>(src);
// }

#endif

template <typename T>
class enable_shared_from_this
#ifndef JAVAPP_ENABLE_GC
    : public std::enable_shared_from_this<T>
#endif
{
public:
    template <typename V=T>
    requires std::is_same_v<V, T>
    shared<V> from_self() {
#ifdef JAVAPP_ENABLE_GC
        return shared<V>{static_cast<T*>(this)};
#else
        return static_pointer_cast<T>(this->shared_from_this());
#endif
    }

    template <typename V=T>
    requires std::is_same_v<V, T>
    shared<V const> from_self() const {
#ifdef JAVAPP_ENABLE_GC
        return shared<V const>{static_cast<T* const>(this)};
#else
        return static_pointer_cast<T const>(this->shared_from_this());
#endif
    }

    template <typename V>
    requires (std::derived_from<V, T> && !std::is_same_v<V, T>)
    shared<V> from_self() {
#ifdef JAVAPP_ENABLE_GC
        return dynamic_pointer_cast<V>(shared<T>{static_cast<T*>(this)});
#else
        return dynamic_pointer_cast<V>(this->shared_from_this());
#endif
    }

    template <typename V>
    requires (std::derived_from<V, T> && !std::is_same_v<V, T>)
    shared<V const> from_self() const {
#ifdef JAVAPP_ENABLE_GC
        return dynamic_pointer_cast<V const>(shared<T const>{static_cast<T* const>(this)});
#else
        return dynamic_pointer_cast<V const>(this->shared_from_this());
#endif
    }

    enable_shared_from_this& operator=(const enable_shared_from_this& rhs) noexcept = default;
};

using byte = char;

extern lateinit_stack _G_stack;

extern std::unordered_map<std::type_index, internals::typeData> typeMap;

struct monitor {
    std::recursive_mutex _monitor_mutex;
    std::condition_variable_any _monitor_cond;
};

struct optional_monitor_container {
    static std::mutex mtx;
    monitor* m{nullptr};

    ~optional_monitor_container();

    monitor* get();
};

class Object : public enable_shared_from_this<Object> {
    optional_monitor_container monitor;

    RTTR_ENABLE();

public:
    virtual ~Object() = default;

protected:
    virtual shared<Object> clone();

public:
    template <typename T>
    static void registerType() {
        const auto& tid = typeid(T);
        const auto id = std::type_index(tid);
        typeMap[id] = {
            sizeof(T),
            tid.name(),
            [](void* ptr) -> Object* {
                return static_cast<Object*>(reinterpret_cast<T*>(ptr));
            }
        };
    }

    static internals::typeData& getType(const std::type_index &idnx) {
        auto r = typeMap.find(idnx);
        if (r == typeMap.end())
            throw std::runtime_error("the expected type was not registered!");
        return r->second;
    }

    virtual void lateinit();

    void notify();

    void notifyAll();

    void wait();

    void wait(long long timeoutMillis);

    void wait(long long timeoutMillis, int nanos);

    virtual int hashCode();

    virtual shared<String> toString();

    virtual bool equals(Object* obj);

    /**
     * @brief THIS IS NOT DESIGNED TO BE HIDDEN, DO NOT HIDE
     * @param obj the object to compare
     */
    template <class T>
    requires std::derived_from<T, Object>
    bool equals(shared<T> obj) {
        return this->equals(obj.get());
    }

    /**
     * @brief THIS IS NOT DESIGNED TO BE HIDDEN, DO NOT HIDE
     */
    template <class T>
    requires std::derived_from<T, Object>
    bool instanceof() {
        auto* n = dynamic_cast<T*>(this);
        return n != nullptr;
    }

    internals::deferable synchronize();

    bool equals_overload_resolve(Object* obj);
};

extern std::unordered_map<std::string, std::shared_ptr<String>> stringMap;

void check_gc();

template <typename T, typename... Args>
shared<T> alloc(Args&&... args) {
#ifdef JAVAPP_ENABLE_GC
    void* memory = gcmalloc(sizeof(T));

    check_gc();

    T* sptr = new (memory) T(std::forward<Args>(args)...);


    auto s = shared<T>(sptr);

    if (s.get() == nullptr)
        throw std::runtime_error("Out of memory!");

    if constexpr(std::derived_from<T, Object>) {
        s->lateinit();
        gcsetbase(sptr, static_cast<Object *>(sptr));
    }

    return s;
#else
    if constexpr (!std::derived_from<T, Object>)
        return std::make_shared<T>(std::forward<Args>(args)...);

    auto sptr = shared<T>(new T(std::forward<Args>(args)...));
    if constexpr (std::derived_from<T, Object>) {
        sptr->lateinit();
    }
    return sptr;
#endif
}

#define DEFINE_SHARED_EQUALS            \
template <class T>                      \
requires std::derived_from<T, Object>   \
bool equals(shared<T> obj) {            \
    return this->equals(obj.get());     \
}

template <typename T>
std::vector<shared<Object>> decay_vec(const std::vector<shared<T>>& source) {
    std::vector<shared<Object>> result;

    for (const auto& element : source)
        result.push_back(element);

    return result;
}

template<typename _Rep, typename _Period> void  sleep_for(const std::chrono::duration<_Rep, _Period>& __rtime) {
    if (__rtime < std::chrono::nanoseconds(100)) {
        // if it's a short sleep will just do it
        std::this_thread::sleep_for(__rtime);
        return;
    }
}

#define FOREACH(type, variable, iterable, body)                                     \
{                                                                                   \
    shared<java::util::Iterator> it = iterable->iterator();                         \
    while (it->hasNext()) {                                                         \
        const shared<type> variable = dynamic_pointer_cast<type>(it->next());       \
        body                                                                        \
    }                                                                               \
}

#define null nullptr

#define lambda [=, self=from_self<std::remove_reference_t<decltype(*this)>>()]

#define pself from_self<std::remove_reference_t<decltype(*this)>>()

[[noreturn]] void panic(const char* msg);

[[noreturn]] void panic(const std::string& msg);

#ifdef NDEBUG
#define assert_once(msg) {}
#else
#define assert_once(msg) {          \
    static int reach_counter = 0;   \
    if (reach_counter++ >= 1)       \
        panic(msg);                 \
}
#endif

shared<String> operator ""_s(const char* str, std::size_t len);

int main(int argc, char** argv);

int jmain(int argc, char** argv);
