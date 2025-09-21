#include "Number.h"

Number::Number() = default;

byte Number::byteValue() {
    return static_cast<byte>(intValue());
}

short Number::shortValue() {
    return static_cast<short>(intValue());
}
