#include "garbage_collection.h"

#include <Mallocator.h>
#include <sys/mman.h>

#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
#include <chrono>
#include <iostream>
#endif

namespace javapp {
    garbage_collection* garbage_collection::singleton;

        garbage_collection::garbage_collection() {
        if (singleton != nullptr) {
            throw std::runtime_error("garbage_collection singleton requirement violated!");
        }
        singleton = this;
    }

    garbage_collection::~garbage_collection() = default;

    void garbage_collection::gc_maybe() {
        auto tid = std::this_thread::get_id();
        if (_thread_stop.stop_requested()) {
            // first, recycle into a new stop token stop source pair
            auto& source = _G_stop_sources[tid] = std::stop_source{};
            auto& src = _G_stop_tokens[tid] = source.get_token();
            _thread_stop = src;

            if (_gc_done)
                return; // we might have reached this after a GC was previously aborted (probably because of us)

            // fill the stop information for gc to use
            auto& info = _G_stop_information[tid] = thread_information{};
            getcontext(&info.context);

            // check if we should notify the GC worker
            std::unique_lock _lock(_gc_mutex); // lock the mutex
            _gc_at_barrier++; // increment at barrier count
            // check if everyone is here
            if (_G_thread_count == _gc_at_barrier)
                _gc_cond.notify_all();

            // wait for GC worker to finish by waiting on _gc_ok and _gc_done.
            _gc_ok.wait(_lock, [&]() -> bool {
                return _gc_done;
            });

            _gc_at_barrier--;
        }
    }

    void garbage_collection::stop_the_world() {
        using namespace std::chrono_literals;

        _gc_done = false; // unset the GC done flag.

        _gc_stop.notify_all(); // for the sleeping threads.

        start = std::chrono::system_clock::now();
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        std::cout << std::format("Stop-The-World GC pass started at {}", start) << std::endl;
#endif
        for (auto& [_, source] : _G_stop_sources)
            (void)source.request_stop();
    }

    void garbage_collection::resume_the_world() {
        using namespace std::chrono_literals;

        _gc_done = true; // set GC done.

        end = std::chrono::system_clock::now();
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        std::cout << std::format("Stop-The-World GC pass done at {}, taking {}", end, end - start) << std::endl;
#endif

        _gc_ok.notify_all();
    }

    bool garbage_collection::wait_gc(std::unique_lock<std::mutex> &_lock) const {
        using namespace std::chrono_literals;

#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
        std::cout << std::format("Stop-The-World GC pass waited for stop for {} before all threads reached", std::chrono::system_clock::now() - start) << std::endl;
#endif

        while (true) {
            _gc_cond.wait_for(_lock, 100ms);
            if (_G_thread_count == _gc_at_barrier)
                return true;
            if (std::chrono::system_clock::now() - start > 100ms) {
                // we waited too long! One of the threads is probably dead/tight loop, let's just give up.
#ifdef JAVAPP_ENABLE_INTERNAL_ASSERTIONS
                std::cerr << "Stop-The-World GC pass waited for stop for more than 100ms! Aborting!" << std::endl;
#endif
                return false;
            }
        }
    }

    void garbage_collection::gc(std::unique_lock<std::mutex> &_lock) {
        stop_the_world();

        bool wait_stat = wait_gc(_lock);

        if (!wait_stat) {
            // gave up
            resume_the_world();
            return;
        }

        mark();
        sweep();

        // reset sinceLastGC
        sinceLastGC.store(0);

        resume_the_world();
    }

    void garbage_collection::mark() {
    }

    void garbage_collection::sweep() {
    }

    bool garbage_collection::want_gc() {
        auto slgc = sinceLastGC.load();
        auto mas = memAllocSum.load();
        if ((mas > 1024*8 && static_cast<float>(slgc) > 0.25 * static_cast<float>(mas)) ||
            std::chrono::system_clock::now() - singleton->last_gc_time > std::chrono::hours(1)) {
            // more than 8KiB on heap and more than 25% of the last heap size was allocated since.
            return true;
        }

        return false;
    }

    void garbage_collection::worker() {
        using namespace std::chrono_literals;

        last_gc_time = std::chrono::system_clock::now();

        while (!_program_stop_token.stop_requested()) {
            std::unique_lock _lock(_gc_mutex);
            auto status = run_gc_variable.wait_for(_lock, 100ms);

            if (want_gc()) {
                // let's just check if we want a GC anyways
                gc(_lock);
                continue;
            }

            if (status == std::cv_status::timeout)
                continue;

            if (!not_spurious)
                continue;

            not_spurious = false;

            gc(_lock);
        }
    }

    void garbage_collection::run_gc() {
        // wake up the gc thread
        singleton->run_gc_variable.notify_all();
        singleton->not_spurious = true;
    }
}

#ifdef JAVAPP_ENABLE_GC
#include <xmmintrin.h>

std::mutex pages_lock;
javapp::page_map* pages{nullptr};
javapp::page_map* array_pages{nullptr};
std::vector<javapp::page_map*, Mallocator<javapp::page_map*>> pages_vec;

thread_local javapp::page_map* small_obj_heap;
thread_local javapp::page_map* large_obj_heap;
thread_local javapp::page_map* array_heap;
thread_local bool gc_heap_initd;

#define GC_SMALL_OBJECT_SIZE 256

// these can be changed depending on use case, they are currently set 256KiB for small objects, 2MiB for large
#define GC_SMALL_OBJ_PAGE_SIZE 256*1025     *1ul
#define GC_LARGE_OBJ_PAGE_SIZE 2*1024*1024  *1ul

javapp::page_map* newpage(bool big) {
    std::unique_lock _lock(pages_lock);

    auto sz = big ? GC_LARGE_OBJ_PAGE_SIZE : GC_SMALL_OBJ_PAGE_SIZE;

    void* page = mmap(nullptr, sz, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (page == MAP_FAILED)
        throw std::bad_alloc{};

    auto* base_tombstone = new ((char*)page + sizeof(javapp::page_map)) javapp::gc_metadata{{}, 0,
            std::numeric_limits<std::uint16_t>::max(), static_cast<std::uint32_t>(sz - sizeof(javapp::gc_metadata) - sizeof(javapp::page_map)), nullptr};

    auto* map = new (page) javapp::page_map {pages, nullptr, reinterpret_cast<std::uintptr_t>(page),
        sz, sz - sizeof(javapp::gc_metadata), sz - sizeof(javapp::gc_metadata),
        {1, false, big ? javapp::SIZE_BIGOBJ : javapp::SIZE_SMALLOBJ},
        nullptr, base_tombstone, (std::uint64_t*)calloc(sz / 64, 1)};

    pages->prev = map;

    memAllocSum += sz;

    pages = map;

    pages_vec.push_back(pages);

    std::sort(pages_vec.begin(), pages_vec.end());

    if (big)
        large_obj_heap = map;
    else
        small_obj_heap = map;

    return map;
}

void *gcmalloc(std::size_t count) {
    if (count > std::numeric_limits<std::uint32_t>::max()) {
        // what in the world???
        // what brain damage did you do to fill such a large object with what is effectively raw pointers?
        panic("ASSUMPTION VIOLATION: why are objects > uintmax bytes in size??? (you'd need a lot of pointer fields to fill that!)\n"
              "HELP: consider using ref counting mode (JAVAPP_ENABLE_GC=OFF), where such problems don't exist\n"
              "NOTE: arrays do not go on the GC'd heap, this is purely object size, perhaps this backtrace can help you know more?");
    }

    // round count up to the nearest 8
    count = (count + 7) & (-8);

    sinceLastGC += count;

    // pick page
    javapp::page_map* map = count <= GC_SMALL_OBJECT_SIZE ? small_obj_heap : large_obj_heap;

    if (count > GC_SMALL_OBJ_PAGE_SIZE)
        return gcamalloc(1, count);

    bool wantpage = true;

    while (wantpage) {
        if (!map) {
            wantpage = false; // if we still can't then something else has happened

            // try to make a new page
            map = newpage(count <= GC_SMALL_OBJECT_SIZE);

            // reset this, we probably should work through the pages again
            if (count <= GC_SMALL_OBJECT_SIZE)
                small_obj_heap = pages;
            else
                large_obj_heap = pages;
        }

        // get lock
        while (map->flags.page_lock_flag.test_and_set(std::memory_order_acquire))
            _mm_pause();

        javapp::gc_metadata* block;
        javapp::gc_metadata* last;

        if (!map->tombstone_chain)
            goto skip;

        block = map->tombstone_chain->next;
        last = map->tombstone_chain;

        if (map->largest_contiguous < count) {
            // no way we're fitting the data in here
            goto skip;
        }

        if (map->tombstone_chain->block_size >= count) {
            // take this block
            javapp::gc_metadata* block = map->tombstone_chain;

            if (map->tombstone_chain->block_size > count) {
                // make new tombstone after block
                void* newmeta = reinterpret_cast<char*>(block + 1) + count;
                auto* newblock = new (newmeta)javapp::gc_metadata();

                newblock->bitfield = {};
                newblock->object_base_offset = std::numeric_limits<std::uint16_t>::max(); // set as sentinel
                newblock->block_size = block->block_size - sizeof(javapp::gc_metadata) - count;

                // update tombstone chain
                map->tombstone_chain = newblock;
                newblock->next = block->next;
            } else {
                // update tombstone
                map->tombstone_chain = block->next;
            }

            // update map's alive chain
            block->next = map->alive_chain;
            map->alive_chain = block;

            // update page metadata
            map->available_space -= count; // didn't allocate any more gc_metadatas
            map->largest_contiguous = std::min(count, map->largest_contiguous);

            // update allocation bitmap
            auto offset = (uintptr_t)((char*)(block + 1) - map->page_base);
            size_t slot_index = offset / 8;
            size_t word_index = slot_index / 64;
            size_t bit_index  = slot_index % 64;
            map->alloc_map[word_index] |= (1ULL << bit_index);

            // free the lock
            map->flags.page_lock_flag.clear(std::memory_order_release);

            return block + 1; // immediately after the block is our newly acquired data
        }

        while (block != nullptr) {
            if (map->tombstone_chain->block_size >= count) {
                if (map->tombstone_chain->block_size > count) {
                    void* newmeta = reinterpret_cast<char*>(block + 1) + count;
                    javapp::gc_metadata* newblock = new (newmeta)javapp::gc_metadata();

                    newblock->bitfield = {};
                    newblock->object_base_offset = std::numeric_limits<std::uint16_t>::max();
                    newblock->block_size = block->block_size - sizeof(javapp::gc_metadata) - count;

                    last->next = newblock;
                    newblock->next = block->next;
                } else
                    last->next = block->next;

                block->next = map->alive_chain;
                map->alive_chain = block;

                map->available_space -= count;
                map->largest_contiguous = std::min(count, map->largest_contiguous);

                uintptr_t offset     = reinterpret_cast<uintptr_t>(reinterpret_cast<char *>(block + 1) - map->page_base);
                size_t    slot_index = offset / 8;
                size_t    word_index = slot_index / 64;
                size_t    bit_index  = slot_index % 64;
                map->alloc_map[word_index] |= (1ULL << bit_index);

                map->flags.page_lock_flag.clear(std::memory_order_release);

                return block + 1;
            }

            last = block;
            block = block->next;
        }

    skip:
        // free the lock
        map->flags.page_lock_flag.clear(std::memory_order_release);

        map = map->next;
    }

    throw std::bad_alloc{};
}

void * gcamalloc(std::size_t elem_size, std::size_t count) {
    std::size_t total = elem_size * count;
    total = (total + 7) & (-8);
    total += sizeof(javapp::gc_metadata);
    total +=

    if (total <= GC_SMALL_OBJ_PAGE_SIZE) {
        // we might find a space in the big object heap
        if (void *res = gcmalloc(total))
            return res;
    }

    // allocate nearest page size.
    std::unique_lock _lock(pages_lock);

    void* page = mmap(nullptr, total, PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (page == MAP_FAILED)
        throw std::bad_alloc{};

    auto* base_tombstone = new ((char*)page + sizeof(javapp::page_map)) javapp::gc_metadata{{}, 0,
            std::numeric_limits<std::uint16_t>::max(), static_cast<std::uint32_t>(total), nullptr};

    auto* map = new (page) javapp::page_map {pages, nullptr, reinterpret_cast<std::uintptr_t>(page),
        sz, sz - sizeof(javapp::gc_metadata), sz - sizeof(javapp::gc_metadata),
        {1, false, big ? javapp::SIZE_BIGOBJ : javapp::SIZE_SMALLOBJ},
        nullptr, base_tombstone, (std::uint64_t*)calloc(sz / 64, 1)};

    pages->prev = map;

    auto* base_tombstone = new (page) javapp::gc_metadata{{}, 0,
            std::numeric_limits<std::uint16_t>::max(), static_cast<std::uint32_t>(total), nullptr};

    auto* map = new (base_tombstone) javapp::page_map {array_pages, nullptr, reinterpret_cast<std::uintptr_t>(page),
        total, total - sizeof(javapp::gc_metadata), total - sizeof(javapp::gc_metadata),
        {1, false, javapp::SIZE_ARRAY},
        base_tombstone, nullptr, nullptr};

    array_pages->prev = map;

    array_pages = map;

    pages_vec.push_back(pages);

    std::sort(pages_vec.begin(), pages_vec.end());

    return base_tombstone + 1;
}

javapp::page_map* gcpage(void* addr) {
    auto addr_ptr = reinterpret_cast<std::uintptr_t>(addr);

    std::size_t lo = 0;
    std::size_t hi = pages_vec.size() - 1;

    while (lo <= hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (addr_ptr < pages_vec[mid]->page_base)
            hi = mid - 1;
        else if (addr_ptr > pages_vec[mid]->page_size)
            lo = mid + 1;
        else
            return pages_vec[mid];
    }
    return nullptr; // pointer not in a GC page
}

javapp::gc_metadata* gcscan(void* addr) {
    // find the page it might reside in.
    auto* page = gcpage(addr);
    if (!page)
        return nullptr;

    uintptr_t offset = reinterpret_cast<uintptr_t>(page) - page->page_base;
    size_t slot_index = offset / 8;
    size_t word_index = slot_index / 64;
    size_t bit_index  = slot_index % 64;
    uint64_t word = page->alloc_map[word_index];

    uint64_t mask = (1ULL << bit_index) - 1; // all bits below current slot
    word &= mask; // zero out bits above pointer

    while (word == 0) {
        // go to previous word
        if (word_index == 0) {
            // pointer is before first object in page — error!
            return nullptr;
        }
        word_index--;
        word = page->alloc_map[word_index];
    }

    // find the most significant 1 bit
    int msb_index = 63 - __builtin_clzll(word);
    // object start slot
    size_t object_slot_index = word_index * 64 + msb_index;

    // compute object base address
    uintptr_t object_base = page->page_base + object_slot_index * 8;

    return reinterpret_cast<javapp::gc_metadata *>(object_base - sizeof(javapp::gc_metadata));
}

void gcfree(void *ptr) {
    // TODO: not doing anything is also valid (technically)
    // but not freeing blocks potentially multiple metabytes in size sounds stupid
}

void gcsetbase(void *base, void *objectbase) {
    const auto baseptr = reinterpret_cast<std::uintptr_t>(base);
    const auto objectptr = reinterpret_cast<std::uintptr_t>(objectbase);

    auto diff = objectptr - baseptr;

    if (diff > 65536)
        throw std::runtime_error("How did you get this past the allocator???");

    javapp::gc_metadata* meta = static_cast<javapp::gc_metadata *>(base) - 1;
    meta->object_base_offset = diff;
}

void gcinit() {
    // setup for this thread
    if (!gc_heap_initd) {
        // let's let at most 4 threads use a single page at a time
        if (pages == nullptr) {
            // initialize the first page
            newpage(false);
            newpage(true);
        }
    }
}

void gcshutdown() {
    if (gc_heap_initd) {
        // decrement the page contention flags.
        if (small_obj_heap)
            --small_obj_heap->flags.page_contention;
        if (large_obj_heap)
            --large_obj_heap->flags.page_contention;
        if (array_heap)
            --array_heap->flags.page_contention;
    }
}
#endif
