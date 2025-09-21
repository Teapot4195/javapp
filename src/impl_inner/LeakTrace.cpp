#include "LeakTrace.h"

#ifdef JAVAPP_TRACE_ALLOCS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <unordered_map>

#include <execinfo.h>
#include <cxxabi.h>

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

std::mutex allocMapMutex;
std::unordered_map<void*, AllocInfo, std::hash<void*>, std::equal_to<>, Mallocator<std::pair<void* const, AllocInfo>>> allocMap;

void** buffer = nullptr;

#define BTBUFFER_MAX 500

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

void impl_internal::printLeaks() {
    allocMapMutex.lock();
    if (allocMap.empty()) {
        fprintf(stderr, "LEAK DETECTION: No leaks are detected.\n");
        return;
    }

    fprintf(stderr, "LEAK DETECTION: Detected leaks are as follows:\n");

    size_t total_leaked = 0;

    for (auto& [key, data] : allocMap) {
#ifdef JAVAPP_TRACE_ALLOCS_STACKTRACE
        fprintf(stderr, "LEAK DETECTION: - %p (leaked %lu bytes with alignment %d)\n", key, data.size, static_cast<int>(data.align));
        fprintf(stderr, "LEAK DETECTION:     Allocation trace is as follows (most recent call first):\n");
        char** backtrace = backtrace_symbols(data.backtraceBuffer, data.backtraceBufferSize);
        for (int i = 0; i < data.backtraceBufferSize; i++) {
            if (data.backtraceBuffer[i] == nullptr)
                continue;
            char* demangled = nullptr;
            if (char *left_par = nullptr, *plus = nullptr;
                ((left_par = strchr(backtrace[i], '('))) && ((plus = strchr(left_par, '+')))) {
                char tmp = '\0';
                std::swap(tmp, *plus);
                int status = 0;
                demangled = abi::__cxa_demangle(left_par + 1, nullptr, nullptr, &status);
                std::swap(tmp, *plus);

                if (demangled) {
                    size_t need_extra = strlen(plus), demangled_size = strlen(demangled);
                    char* res = static_cast<char *>(malloc(demangled_size + need_extra + 1));
                    memcpy(res, demangled, demangled_size);
                    memcpy(res + demangled_size, plus, need_extra);
                    res[demangled_size + need_extra] = '\0';
                    free(demangled);

                    demangled = res;
                }
            }
            fprintf(stderr, "LEAK DETECTION:       [%d] %p: %s\n", i, data.backtraceBuffer[i], demangled ? demangled : backtrace[i]);
            free(demangled);
        }
        free(backtrace);
#else
        fprintf(stderr, "LEAK DETECTION: - %p (leaked %lu bytes with alignment %d\n", key, data.size, static_cast<int>(data.align));
#endif
        total_leaked += data.size;
    }

#ifndef JAVAPP_TRACE_ALLOCS_STACKTRACE
    fprintf(stderr, "LEAK DETECTION: Build the project with JAVAPP_TRACE_ALLOCS_STACKTRACE for more information.\n");
#endif
    fprintf(stderr, "LEAK DETECTION: A total of %lu bytes still in use at exit.\n", total_leaked);

    allocMapMutex.unlock();
}

bool init_leakprint = [] {
    atexit(impl_internal::printLeaks);
    return true;
}();
#endif
