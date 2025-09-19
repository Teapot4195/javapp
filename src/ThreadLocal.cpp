#include "ThreadLocal.h"

#include <java/util/function/Supplier.h>

ThreadLocal::ThreadLocal() = default;

shared<Object> ThreadLocal::get() {
    auto lock = this->synchronize();
    const auto res = data.find(std::this_thread::get_id());

    if (res == data.end()) {
        return data[std::this_thread::get_id()] = initialValue();
    }

    return res->second;
}

shared<Object> ThreadLocal::initialValue() {
    return null;
}

class ThreadLocalWithInitial final : public ThreadLocal {
    shared<java::util::function::Supplier> supp;

public:
    explicit ThreadLocalWithInitial(shared<java::util::function::Supplier> supplier) : supp(supplier) {}

    shared<Object> initialValue() override {
        return supp->get();
    }
};

shared<ThreadLocal> ThreadLocal::withInitial(shared<java::util::function::Supplier> supplier) {
    if (supplier == null)
        throw std::runtime_error("THROW NULLPOINTEREXCEPTION!");

    return alloc<ThreadLocalWithInitial>(supplier);
}

void ThreadLocal::set(shared<Object> value) {
    auto lock = this->synchronize();
    auto& res = data[std::this_thread::get_id()];
    res = value;
}

void ThreadLocal::remove() {
    auto lock = this->synchronize();
    data.erase(std::this_thread::get_id());
}
