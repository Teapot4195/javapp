#pragma once

#include <atomic>
#include <Object.h>
#include <string>

namespace infra {
    extern std::atomic_size_t passed, failed;

    inline void pass() {
        ++passed;
    }

    void fail();

    void fail(const std::string &msg);

    // TODO: upgrade this to throwables and let us print the actual stacktrace
    void unexpected(std::exception& e);

    inline void check(bool cond) {
        if (cond)
            pass();
        else
            fail();
    }

    inline void check(bool cond, const std::string &msg) {
        if (cond)
            pass();
        else
            fail(msg);
    }

    inline void icheck(bool cond) {
        if (!cond)
            pass();
        else
            fail();
    }

    inline void icheck(bool cond, const std::string &msg) {
        if (!cond)
            pass();
        else
            fail(msg);
    }

    void equal(shared<Object> x, shared<Object> y);
}
