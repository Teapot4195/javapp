#pragma once

#include <Object.h>

class Runnable : public virtual Object {
    std::function<void()> inner;

public:
    ~Runnable() override = default;

    /**
     * Implicit conversion function
     * @tparam F function type
     * @param func the inner function
     */
    template<typename F>
    Runnable(F&& func) : inner(std::forward<F>(func)) {}

    virtual void run();
};
