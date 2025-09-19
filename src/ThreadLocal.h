#pragma once

#include <map>
#include <Object.h>

namespace java::util::function {
    class Supplier;
}

class ThreadLocal : public virtual Object {
    std::map<std::thread::id, shared<Object>> data;
    // std::mutex mutex; // it is possible to save a mutex here, we can use the mutex inside Object instead.
public:
    ThreadLocal();

    virtual shared<Object> get();

protected:
    virtual shared<Object> initialValue();

public:
    virtual void remove();

    virtual void set(shared<Object> value);

    static shared<ThreadLocal> withInitial(shared<java::util::function::Supplier> supplier);
};
