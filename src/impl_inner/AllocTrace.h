#pragma once

#include <cstdlib>
#include <mutex>
#include <unordered_map>

template<class T>
struct Mallocator
{
    typedef T value_type;

    Mallocator() = default;

    template<class U>
    constexpr explicit Mallocator(const Mallocator <U>&) noexcept {}

    [[nodiscard]] T* allocate(const std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();

        if (auto p = static_cast<T*>(malloc(n * sizeof(T))))
        {
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* p, const std::size_t _) noexcept
    {
        free(p);
    }
};

struct AllocInfo {
    size_t size;
    std::align_val_t align;
#ifdef JAVAPP_TRACE_ALLOCS_STACKTRACE
    void** backtraceBuffer;
    size_t backtraceBufferSize;

    ~AllocInfo() {
        free(backtraceBuffer);
    }
#endif
};

extern std::mutex allocMapMutex;
extern std::unordered_map<void*, AllocInfo, std::hash<void*>, std::equal_to<>, Mallocator<std::pair<void* const, AllocInfo>>> allocMap;
