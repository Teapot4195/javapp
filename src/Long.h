#pragma once

#include "Object.h"

class Long : public virtual Object {
public:
    static constexpr long long MAX_VALUE = std::numeric_limits<long long>::max();
    static constexpr long long MIN_VALUE = std::numeric_limits<long long>::min();
};
