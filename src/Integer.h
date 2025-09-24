#pragma once

#include <Comparable.h>
#include <Number.h>
#include <Object.h>

#warning Interim implementation of Integer!

class Integer : public virtual Object, public virtual Number, public virtual Comparable {
    int boxed_value;

public:
    static constexpr int BYTES = sizeof(int);
    static constexpr int MAX_VALUE = std::numeric_limits<int>::max();
    static constexpr int MIN_VALUE = std::numeric_limits<int>::min();
    static constexpr int SIZE = BYTES * 8;
    // todo: static constexpr Class<Integer> TYPE;

    [[deprecated("It is rarely appropriate to use this constructor")]]
    explicit Integer(int value);

    [[deprecated("It is rarely appropriate to use this constructor")]]
    explicit Integer(shared<String> value);

    static int bitCount(int value);

    byte byteValue() override;

    static int compare(int x, int y);

    int compareTo(shared<Object> o) override;

    static int compareUnsigned(int x, int y);

    static int compress(int i, int mask);

    static shared<Integer> decode(shared<String> value);

    static int divideUnsigned(int dividend, int divisor);

    double doubleValue() override;

    bool equals(Object *obj) override;

    DEFINE_SHARED_EQUALS

    static int expand(int i, int mask);

    float floatValue() override;

    // static shared<Integer> getInteger(shared<String> nm);

    // static shared<Integer> getInteger(shared<String> nm, int val);

    // static shared<Integer> getInteger(shared<String> nm, shared<Integer> val);

    int hashCode() override;

    static int hashCode(int value);

    static int highestOneBit(int i);

    int intValue() override;

    long long longValue() override;

    static int lowestOneBit(int i);

    static int max(int a, int b);

    static int min(int a, int b);

    static int numberOfLeadingZeros(int i);

    static int numberOfTrailingZeros(int i);

    // static int parseInt(shared<CharSequence> s, int beginIndex, int endIndex, int radix);

    // static int parseInt(String s)

    // static int parseInt(String s, int radix)

    // static int parseUnsignedInt(CharSequence s, int beginIndex, int endIndex, int radix)

    // static int parseUnsignedInt(String s)

    // static int parseUnsignedInt(String s, int radix)

    static int remainderUnsigned(int dividend, int divisor);

    static int reverse(int i);

    static int reverseBytes(int i);

    static int rotateLeft(int i, int distance);

    static int rotateRight(int i, int distance);

    short shortValue() override;

    static int signum(int i);

    static int sum(int a, int b);

    // static shared<String> toBinaryString(int i);

    // static shared<String> toHexString(int i);

    // static shared<String> toOctalString(int i);

    shared<String> toString() override;

    static shared<String> toString(int i);

    // static shared<String> toString(int i, int radix);

    static long long toUnsignedLong(int x);

    static shared<String> toUnsignedString(int i);

    // static shared<String> toUnsignedString(int i, int radix);

    static shared<Integer> valueOf(int i);

    // static shared<Integer> valueOf(shared<String> s);

    // static shared<Integer> valueOf(shared<String> s, int radix);

private:
    __always_inline static int parallelSuffix(int maskCount) {
        int maskPrefix = maskCount ^ (maskCount << 1);
        maskPrefix = maskPrefix ^ (maskPrefix << 2);
        maskPrefix = maskPrefix ^ (maskPrefix << 4);
        maskPrefix = maskPrefix ^ (maskPrefix << 8);
        maskPrefix = maskPrefix ^ (maskPrefix << 16);
        return maskPrefix;
    }

    // TODO: automatically clean up this pool every now and then!
    static std::unordered_map<int, shared<Integer>> intpool;
};
