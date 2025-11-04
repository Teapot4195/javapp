#include "AllocTrace.h"

#include <cstring>

#include <execinfo.h>

#define BTBUFFER_MAX 500

std::mutex allocMapMutex;
std::unordered_map<void*, AllocInfo, std::hash<void*>, std::equal_to<>, Mallocator<std::pair<void* const, AllocInfo>>> allocMap;

extern void** buffer;

__always_inline void** backtrace_buffer(size_t* bufsize) {
    if (buffer == nullptr)
        buffer = static_cast<void **>(malloc(BTBUFFER_MAX * sizeof(void*)));
    int count = backtrace(buffer, BTBUFFER_MAX);
    *bufsize = count * sizeof(void*);

    // reallocate this into a custom sized buffer to reduce mem use
    void** result_buffer = static_cast<void**>(malloc(BTBUFFER_MAX * sizeof(void*)));
    memcpy(result_buffer, buffer, count * sizeof(void*));
    return result_buffer;
}

__always_inline void leakDetectRegister(void* alloc_location, std::size_t count, std::align_val_t alignment) {
#ifdef JAVAPP_TRACE_ALLOCS
    allocMapMutex.lock();
    auto& data = allocMap[alloc_location];
    data.size = count;
    data.align = alignment;

#ifdef JAVAPP_TRACE_ALLOCS_STACKTRACE
    data.backtraceBuffer = backtrace_buffer(&data.backtraceBufferSize);
#endif
    allocMapMutex.unlock();
#endif
}

void* operator new(std::size_t count) {
    void* ptr = malloc(count);
    if (ptr == nullptr)
        throw std::bad_alloc();

    leakDetectRegister(ptr, count, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));

    return ptr;
}

void* operator new(std::size_t count, std::align_val_t alignment) {
    void* ptr = malloc(count);
    if (ptr == nullptr)
        throw std::bad_alloc();

    leakDetectRegister(ptr, count, alignment);

    return ptr;
}

void* operator new[](std::size_t count) {
    void* ptr = malloc(count);
    if (ptr == nullptr)
        throw std::bad_alloc();

    leakDetectRegister(ptr, count, static_cast<std::align_val_t>(__STDCPP_DEFAULT_NEW_ALIGNMENT__));

    return ptr;
}

void* operator new[](std::size_t count, std::align_val_t alignment) {
    void* ptr = malloc(count);
    if (ptr == nullptr)
        throw std::bad_alloc();

    leakDetectRegister(ptr, count, alignment);

    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        allocMapMutex.lock();
        allocMap.erase(ptr);
        allocMapMutex.unlock();
    }
    free(ptr);
}

void operator delete(void* ptr, std::size_t _) noexcept {
    if (ptr != nullptr) {
        allocMapMutex.lock();
        allocMap.erase(ptr);
        allocMapMutex.unlock();
    }
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        allocMapMutex.lock();
        allocMap.erase(ptr);
        allocMapMutex.unlock();
    }
    free(ptr);
}

void operator delete[](void* ptr, std::size_t _) noexcept {
    if (ptr != nullptr) {
        allocMapMutex.lock();
        allocMap.erase(ptr);
        allocMapMutex.unlock();
    }
    free(ptr);
}
