#pragma once

#include <Object.h>

class Comparable : public virtual Object {
public:
    ~Comparable() override = default;

    virtual int compareTo(shared<Object> o) = 0;
};
