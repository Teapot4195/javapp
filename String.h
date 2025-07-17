#pragma once

#include "Object.h"

class String : public virtual Object {
public:
    ~String() override = default;

    /**
     * @brief implicitly allow conversion from std::string
     * @param str the string to implicitly convert from
     */
    explicit String(const std::string& str);

    /**
     * @brief implicitly allow conversion from a string literal
     * @param str the string literal to implicitly convert from
     */
    explicit String(const char* str);
};
