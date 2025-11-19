#pragma once

#include <rttr/registration_friend.h>

#include "Object.h"

template <typename T>
struct is_obj : std::false_type {};

template <typename T>
struct is_obj<shared<T>> : std::true_type {
    using type = T;
};

class TypeErasedArray : public virtual Object {
protected:
    int length;
    void* data;

    TypeErasedArray() : length(0), data(nullptr) {}

    TypeErasedArray(int length, void* data) : length(length), data(data) {}

    RTTR_REGISTRATION_FRIEND;
};

template <typename T = shared<Object>>
class Array final : public virtual Object, public TypeErasedArray {
    RTTR_ENABLE(Object);

public:
    ~Array() override {
#ifndef JAVAPP_ENABLE_GC
        // under GC it's not our problem :xdd:
        delete[] ((T*)data);
#endif
    }

    explicit Array(shared<Array> last) : TypeErasedArray(last->length, static_cast<T *>(
#ifdef JAVAPP_ENABLE_GC
        gcamalloc(sizeof(T), last->length)
#else
        new T[last->length]
#endif
        )) {
        for (int i = 0; i < length; i++)
            ((T*)data)[i] = last->get_data()[i];
    }

    Array() {}

    explicit Array(std::vector<T> parent) : TypeErasedArray(parent.size(), static_cast<T *>(
#ifdef JAVAPP_ENABLE_GC
        gcamalloc(sizeof(T), parent.size())
#else
        new T[parent.size()]
#endif
        )) {
        for (size_t i = 0; i < parent.size(); i++) {
            ((T*)data)[i] = parent[i];
        }
    }

    explicit Array(const int size) : TypeErasedArray(size, static_cast<T *>(
#ifdef JAVAPP_ENABLE_GC
        gcamalloc(sizeof(T), size)
#else
        new T[size]
#endif
    )) {
        if constexpr(is_obj<T>::value && std::is_default_constructible_v<typename is_obj<T>::type>) {
            for (int i = 0; i < length; i++)
                ((T*)data)[i] = alloc<typename is_obj<T>::type>();
        }
    }

    void resize(int size) {
#ifdef JAVAPP_ENABLE_GC
        auto* newdata = (T*)gcamalloc(sizeof(T), size);
#else
        auto* newdata = new T[size];
#endif

        for (int i = 0; i < length; i++) {
            newdata[i] = ((T*)data)[i];
        }

        length = size;

#ifdef JAVAPP_ENABLE_GC
        gcfree(data);
#else
        delete[] (T*)data;
#endif
        data = newdata;
    }

    T* get_data() {
        return static_cast<T*>(data);
    }

    int get_length() const {
        return length;
    }

    void set_length(int length) {
        TypeErasedArray::length = length;
    }
};
