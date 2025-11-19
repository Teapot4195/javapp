#pragma once

#include <atomic>
#include <cstdlib>
#include <Mallocator.h>
#include <mutex>
#include <typeindex>
#include <unordered_map>

class Object;

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

// void insert_sliplist_allocation(Object* address, std::type_index index);
//
// void remove_sliplist_allocation(Object* address);

#ifdef JAVAPP_ENABLE_GC
void* gcmalloc(std::size_t count);

// malloc a GC managed array.
void* gcamalloc(std::size_t elem_size, std::size_t count);

void gcfree(void* ptr);

void gcsetbase(void* base, void* objectbase);

void gcinit();

void gcshutdown();
#endif

extern std::atomic_size_t memAllocSum;
extern std::atomic_size_t sinceLastGC;

extern std::mutex allocMapMutex;
extern std::unordered_map<void*, AllocInfo, std::hash<void*>, std::equal_to<>, Mallocator<std::pair<void* const, AllocInfo>>> allocMap;
