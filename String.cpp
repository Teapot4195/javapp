
#include "String.h"

String::String(const std::string &str) : backing(str) {
}

String::String(const char *str) : backing(str) {
}

bool String::equals(Object *obj) {
    if (!obj->instanceof<String>())
        return false;
    if (const auto* other = dynamic_cast<String*>(obj); other->backing == backing)
        return true;
    return false;
}

std::string & String::get_backing() {
    return backing;
}
