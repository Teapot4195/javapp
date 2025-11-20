#include "Boolean.h"

#include <String.h>

// Suppress deprecation of the constructors
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
const shared<Boolean> Boolean::FALSE = alloc<Boolean>(false), Boolean::TRUE = alloc<Boolean>(true);
#pragma GCC diagnostic pop

const shared<String> Boolean::STR_TRUE = alloc<String>("true"), Boolean::STR_FALSE = alloc<String>("false");

Boolean::Boolean(bool value) : value(value) {}

Boolean::Boolean(const shared<String> &s) : value(parseBoolean(s)) {}

bool Boolean::booleanValue() const {
    return value;
}

int Boolean::compare(bool x, bool y) {
    if (x == y)
        return 0;
    if (!x && y)
        return -1;
    if (x && !y)
        return 1;
    std::unreachable();
}

int Boolean::compareTo(shared<Object> b) {
    auto cast = dynamic_pointer_cast<Boolean>(b);
    return compare(value, cast->value);
}

bool Boolean::equals(Object *obj) {
    if (!obj)
        return false;
    if (!obj->instanceof<Boolean>())
        return false;

    const auto* other = dynamic_cast<Boolean*>(obj);

    return other->value == value;
}

int Boolean::hashCode() {
    return hashCode(value);
}

int Boolean::hashCode(bool value) {
    if (value)
        return 1231;
    return 1237;
}

bool Boolean::logicalAnd(bool a, bool b) {
    return a && b;
}

bool Boolean::logicalOr(bool a, bool b) {
    return a || b;
}

bool Boolean::logicalXor(bool a, bool b) {
    return a ^ b;
}

bool Boolean::parseBoolean(const shared<String> &name) {
    if (name != nullptr) {
        std::string v = name->get_backing();
        std::ranges::transform(v, v.begin(), ::tolower);
        if (v == "true")
            return true;
    }
    return false;
}

shared<String> Boolean::toString() {
    return toString(value);
}

shared<String> Boolean::toString(bool value) {
    if (value)
        return STR_TRUE;
    return STR_FALSE;
}

shared<Boolean> Boolean::valueOf(bool value) {
    if (value)
        return TRUE;
    return FALSE;
}

shared<Boolean> Boolean::valueOf(const shared<String> &s) {
    return valueOf(parseBoolean(s));
}
