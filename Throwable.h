#pragma once

#include <stdexcept>

#include "Object.h"

class Throwable : public virtual Object, public virtual std::exception {
public:
    ~Throwable() override = default;
};
