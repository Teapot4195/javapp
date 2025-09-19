#pragma once

#include <Object.h>

class Double : public virtual Object {
    double wrapped = 0;

public:
    static constexpr int BYTES = sizeof(double);
    static constexpr int MAX_EXPONENT = std::numeric_limits<double>::max_exponent;
    static constexpr double MAX_VALUE = std::numeric_limits<double>::max();
    static constexpr int MIN_EXPONENT = std::numeric_limits<double>::min_exponent;
    static constexpr double MIN_NORMAL = std::numeric_limits<double>::lowest();
    static constexpr double MIN_VALUE = std::numeric_limits<double>::min();
    static constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
    static constexpr double POSITIVE_INFINITY = std::numeric_limits<double>::infinity();
    static constexpr double NEGATIVE_INFINITY = -std::numeric_limits<double>::infinity();
    static constexpr int PRECISION = std::numeric_limits<double>::digits;
    static constexpr int SIZE = sizeof(double) * 8;
};
