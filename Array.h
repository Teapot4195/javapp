#pragma once

#include "Object.h"

template <typename T = shared<Object>>
class Array final : public virtual Object {
public:
    int length;
    T* data;

    ~Array() override {
        delete[] data;
    }

    explicit Array(shared<Array> last) : length(last->length) {
        data = new T[length];
        for (int i = 0; i < length; i++) {
            data[i] = last->data[i];
        }
    }

    Array() : length(0), data(nullptr) {}

    explicit Array(std::vector<T> parent) : length(parent.size()) {
        data = new T[parent.size()];
        for (int i = 0; i < parent.size(); i++) {
            data[i] = parent[i];
        }
    }

    void resize(int size) {
        auto* newdata = new T[size];

        for (int i = 0; i < length; i++) {
            newdata[i] = data[i];
        }

        length = size;

        delete[] data;
        data = newdata;
    }
};
