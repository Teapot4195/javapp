#pragma once

#include "Object.h"

class AutoCloseable : public virtual Object {
public:
    ~AutoCloseable() override = default;

    virtual void close() = 0;
};
