#pragma once

#include <Object.h>

namespace javapp {
    class garbage_collection {
        std::chrono::system_clock::time_point start, end, last_gc_time;

        std::condition_variable run_gc_variable;
        bool not_spurious{};

        static garbage_collection* singleton;
    public:
        garbage_collection();

        ~garbage_collection();

        // TODO: perhaps define this in the header and make always inline for less call overhead?
        /**
         * @brief Checks whether to perform the GC pass.
         */
        static void gc_maybe();

        void stop_the_world();

        void resume_the_world();

        bool wait_gc(std::unique_lock<std::mutex> &_lock) const;

        void gc(std::unique_lock<std::mutex> &_lock);

        void mark();

        void sweep();

        static bool want_gc();

        void worker();

        static void run_gc();
    };

#ifdef JAVAPP_ENABLE_GC
    struct gc_bitfield {
        bool maked:1{false};
        bool all_reflected:1{true}; ///< whether all accesses are precise
        bool tombstone:1{false};
        char pad:5;
    }  __attribute__((packed));

    struct gc_metadata {
        gc_bitfield bitfield;
        char padding_{};
        std::uint16_t object_base_offset{}; ///< not all gc managed objects have this! we set uint16 max for sentinel.
        std::uint32_t block_size{};
        gc_metadata* next{nullptr}; // next depends on whether this is the alive or tombstone chain
    } __attribute__((packed));

    static_assert(sizeof(gc_metadata) % sizeof(void*) == 0, "gc_metadata should be multiple of ptr size!");

    enum page_settings : std::uint8_t {
        SIZE_SMALLOBJ = 1,
        SIZE_BIGOBJ = 1 << 1,
        SIZE_ARRAY = 1 << 2,
    };

    struct page_flags {
        std::atomic_char8_t page_contention; // number of threads who are allocating to this block
        std::atomic_flag page_lock_flag;
        page_settings settings;
    };

    struct page_map {
        page_map* next;
        page_map* prev;
        std::uintptr_t page_base;
        std::size_t page_size;
        std::size_t available_space; ///< remaining size of any free space left in this page map, including space of non allocated gc_metadata blocks.
        std::size_t largest_contiguous; /// < the largest available single block not including the gc_metadata that this contiguous chunk needs.

        page_flags flags;

        gc_metadata* alive_chain;
        gc_metadata* tombstone_chain;

        std::uint64_t* alloc_map;
    };
#endif
}