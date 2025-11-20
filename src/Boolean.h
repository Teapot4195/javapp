#pragma once

#include <Comparable.h>
#include <Object.h>

class Boolean final : public virtual Object, public virtual Comparable {
    const bool value;

    static const shared<String> STR_TRUE, STR_FALSE;

public:
    static const shared<Boolean> FALSE, TRUE;

    [[deprecated("It is rarely appropriate to use this constructor.")]]
    explicit Boolean(bool value);

    [[deprecated("It is rarely appropriate to use this constructor.")]]
    explicit Boolean(const shared<String>& s);

    bool booleanValue() const;

    static int compare(bool x, bool y);

    int compareTo(shared<Object> b) override;

    bool equals(Object *obj) override;

    DEFINE_SHARED_EQUALS

    // TODO: implement once properties is implemented
    // Properties is not at all implemented lol
    // static bool getBoolean(const shared<String>& name);

    int hashCode() override;

    static int hashCode(bool value);

    static bool logicalAnd(bool a, bool b);

    static bool logicalOr(bool a, bool b);

    static bool logicalXor(bool a, bool b);

    static bool parseBoolean(const shared<String>& name);

    shared<String> toString() override;

    static shared<String> toString(bool value);

    static shared<Boolean> valueOf(bool value);

    static shared<Boolean> valueOf(const shared<String>& s);
};