#include "Object.h"

#include <format>
#include <iostream>
#include <typeindex>
#include <stacktrace>

#include "String.h"

std::unordered_map<std::type_index, internals::typeData> typeMap;

lateinit_stack _G_stack;

lateinit_stack::~lateinit_stack() {
    // by the time this is deconstructed, we should (probably?) have fully emptied the lateinit stack
    // we use this to check if there are any more frames left in the stack, if there is, we should warn the user

    if (this_frame != nullptr && (this_frame->last != nullptr || this_frame->frame != nullptr)) {
        std::cerr << "[WARN] by the time lateinit stack was deconstructed, there were still frames on the stack!!!" << std::endl;
        std::cerr << "[NOTE] this usually means the program has had a severe issue in code." << std::endl;
        std::cerr << "[NOTE] lateinit stack will now unwind for you." << std::endl;
        // the stack is not empty
        int id = 0;
        auto* cur = this_frame;
        while (cur != nullptr) {
            if (cur->frame != nullptr) {
                std::cerr << std::format("[WARN] {}: {}\n", id++, typeid(*cur->frame).name());
            }
            cur = cur->last;
        }
    }
}

void lateinit_stack::die(std::string type) {
    std::cerr << std::format("[FATAL] [lateinit_stack::die] expecting frame to be downcastable to {}, it was {}", type, typeid(*this_frame->next->frame).name()) << std::endl;
    std::terminate();
}

std::shared_ptr<Object> Object::clone() {
    throw std::runtime_error("clone not implemented");
}

void Object::lateinit() {}

void Object::notify() {
    _monitor_cond.notify_one();
}

void Object::notifyAll() {
    _monitor_cond.notify_all();
}

void Object::wait() {
    using namespace std::chrono_literals;

    std::unique_lock _lock(_monitor_mutex);

    const auto& token = _G_stop_tokens[std::this_thread::get_id()];

    while (true) {
        if (const auto result = _monitor_cond.wait_for(_lock, 5ms); result == std::cv_status::timeout) {
            // check for stop requested, then continue
            if (token.stop_requested())
                throw std::runtime_error("Thread stop requested");
            // TODO: throw interrupted exception
            continue;
        }
        return; // the wait just guarantees one thread locks the monitor, spurious wakeups "should" be fine :clueless:
    }
}

void Object::wait(const long long timeoutMillis) {
    wait(timeoutMillis, 0);
}

void Object::wait(const long long timeoutMillis, const int nanos) {
    using namespace std::chrono_literals;

    const auto until = std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis) + std::chrono::nanoseconds(nanos);

    std::unique_lock _lock(_monitor_mutex);

    const auto& token = _G_stop_tokens[std::this_thread::get_id()];

    while (true) {
        if (const auto result = _monitor_cond.wait_for(_lock, 5ms); result == std::cv_status::timeout) {
            // check for stop requested, then continue
            if (token.stop_requested())
                throw std::runtime_error("Thread stop requested");
            // TODO: throw interrupted exception
            if (std::chrono::system_clock::now() < until)
                continue;
        }
        return; // the wait just guarantees one thread locks the monitor, spurious wakeups "should" be fine :clueless:
    }
}

int Object::hashCode() {
    const auto ind = std::type_index(typeid(*this));

    std::cerr << "[WARN] using default hash type for type: " << typeid(*this).name() << " consider specializing for your type!" << std::endl;
    std::cerr << "[NOTE] the default hash does not take into account padding bytes, this hash may behave incorrectly!" << std::endl;

    const auto buf = reinterpret_cast<char*>(this);

    const auto count = typeMap[ind].size;
    int hash = 5381;
    for (int i = 0; i < count; i++)
        hash = hash * 33 ^ buf[i];

    return hash;
}

shared<String> Object::toString() {
    return alloc<String>(std::format("{}@{:#x}", typeid(*this).name(), reinterpret_cast<std::intptr_t>(this)));
}

bool Object::equals(Object *obj) {
    return this == obj;
}

internals::deferable Object::synchronize() {
    _monitor_mutex.lock();
    return std::move(
        internals::deferable{[this] {
            // as long as this is still alive when we call it, no way to guarantee that, so uuh
            // not my problem anymore :pog:
            this->_monitor_mutex.unlock();
        }}
    );
}
