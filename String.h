#pragma once

#include "Object.h"

class String : public virtual Object {
    std::string backing;

public:
    ~String() override = default;

    /**
     * @brief allow conversion from std::string
     * @param str the string to convert from
     */
    explicit String(const std::string& str);

    /**
     * @brief allow conversion from a string literal
     * @param str the string literal to convert from
     */
    explicit String(const char* str);
};
