#pragma once

#include <limits>
#include <stdexcept>

/**
 * @brief A custom allocator that uses malloc over `new`, this is important in GC mode because we hook `new` calls.
 * @tparam T Type of the allocations
 */
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
