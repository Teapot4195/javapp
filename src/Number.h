#pragma once

#include <Object.h>
#include <java/io/Serializable.h>

class Number : public virtual Object, public virtual java::io::Serializable {
public:
    Number();

    virtual byte byteValue();

    virtual double doubleValue() = 0;

    virtual float floatValue() = 0;

    virtual int intValue() = 0;

    virtual long long longValue() = 0;

    virtual short shortValue();
};
