#include "Integer.h"

#include <String.h>

Integer::Integer(int value) : boxed_value(value) {}

Integer::Integer(shared<String> value) {
    boxed_value = std::stoi(value->get_backing());
}

int Integer::bitCount(int value) {
    return __builtin_popcount(value);
}

byte Integer::byteValue() {
    return boxed_value;
}

int Integer::compare(int x, int y) {
    if (x > y)
        return 1;
    if (x < y)
        return -1;
    return 0;
}

int Integer::compareTo(shared<Object> o) {
    return compare(boxed_value, std::dynamic_pointer_cast<Integer>(o)->boxed_value);
}

int Integer::compareUnsigned(int x, int y) {
    uint x1 = x;
    uint y1 = y;
    if (x1 > y1)
        return 1;
    if (x1 < y1)
        return -1;
    return 0;
}

int Integer::compress(int i, int mask) {
    // See Hacker's Delight (2nd ed) section 7.4 Compress, or Generalized Extract

    i = i & mask; // Clear irrelevant bits
    int maskCount = ~mask << 1; // Count 0's to right

    for (int j = 0; j < 5; j++) {
        // Parallel prefix
        // Mask prefix identifies bits of the mask that have an odd number of 0's to the right
        const int maskPrefix = parallelSuffix(maskCount);
        // Bits to move
        const int maskMove = maskPrefix & mask;
        // Compress mask
        mask = static_cast<int>((mask ^ maskMove) | (static_cast<uint>(maskMove) >> (1 << j)));
        // Bits of i to be moved
        const int t = i & maskMove;
        // Compress i
        i = static_cast<int>((i ^ t) | (static_cast<uint>(t) >> (1 << j)));
        // Adjust the mask count by identifying bits that have 0 to the right
        maskCount = maskCount & ~maskPrefix;
    }
    return i;
}

shared<Integer> Integer::decode(shared<String> value) {
}

int Integer::divideUnsigned(int dividend, int divisor) {
    return static_cast<int>(static_cast<uint>(dividend) / static_cast<uint>(divisor));
}

double Integer::doubleValue() {
    return boxed_value;
}

bool Integer::equals(Object *obj) {
    if (!obj->instanceof<Integer>())
        return false;
    if (obj == this)
        return true;
    const auto other = dynamic_cast<Integer *>(obj);
    return other->boxed_value == boxed_value;
}

int Integer::expand(int i, int mask) {
    // Implementation derived from openjdk/jdk.
    // Save original mask
    int originalMask = mask;
    // Count 0's to right
    int maskCount = ~mask << 1;
    int maskPrefix = parallelSuffix(maskCount);
    // Bits to move
    int maskMove1 = maskPrefix & mask;
    // Compress mask
    mask = static_cast<int>((mask ^ maskMove1) | (static_cast<uint>(maskMove1) >> (1 << 0)));
    maskCount = maskCount & ~maskPrefix;

    maskPrefix = parallelSuffix(maskCount);
    // Bits to move
    int maskMove2 = maskPrefix & mask;
    // Compress mask
    mask = static_cast<int>((mask ^ maskMove2) | (static_cast<uint>(maskMove2) >> (1 << 1)));
    maskCount = maskCount & ~maskPrefix;

    maskPrefix = parallelSuffix(maskCount);
    // Bits to move
    int maskMove3 = maskPrefix & mask;
    // Compress mask
    mask = static_cast<int>((mask ^ maskMove3) | (static_cast<uint>(maskMove3) >> (1 << 2)));
    maskCount = maskCount & ~maskPrefix;

    maskPrefix = parallelSuffix(maskCount);
    // Bits to move
    int maskMove4 = maskPrefix & mask;
    // Compress mask
    mask = static_cast<int>((mask ^ maskMove4) | (static_cast<uint>(maskMove4) >> (1 << 3)));
    maskCount = maskCount & ~maskPrefix;

    maskPrefix = parallelSuffix(maskCount);
    // Bits to move
    int maskMove5 = maskPrefix & mask;

    int t = i << (1 << 4);
    i = (i & ~maskMove5) | (t & maskMove5);
    t = i << (1 << 3);
    i = (i & ~maskMove4) | (t & maskMove4);
    t = i << (1 << 2);
    i = (i & ~maskMove3) | (t & maskMove3);
    t = i << (1 << 1);
    i = (i & ~maskMove2) | (t & maskMove2);
    t = i << (1 << 0);
    i = (i & ~maskMove1) | (t & maskMove1);

    // Clear irrelevant bits
    return i & originalMask;
}

float Integer::floatValue() {
    return static_cast<float>(boxed_value);
}

int Integer::hashCode() {
    return hashCode(boxed_value);
}

int Integer::hashCode(int value) {
    return value;
}

int Integer::highestOneBit(int i) {
    return i & (static_cast<uint>(MIN_VALUE) >> numberOfLeadingZeros(i));
}

int Integer::intValue() {
    return boxed_value;
}

long long Integer::longValue() {
    return boxed_value;
}

int Integer::lowestOneBit(int i) {
    // Hackers delight 2nd ed, Section 2-1
    return i & -i;
}

int Integer::max(int a, int b) {
    return std::max(a, b);
}

int Integer::min(int a, int b) {
    return std::min(a, b);
}

int Integer::numberOfLeadingZeros(int i) {
    // HD, Count leading 0's
    if (i <= 0)
        return i == 0 ? 32 : 0;
    int n = 31;
    if (i >= 1 << 16) { n -= 16; i >>= 16; }
    if (i >= 1 <<  8) { n -=  8; i >>=  8; }
    if (i >= 1 <<  4) { n -=  4; i >>=  4; }
    if (i >= 1 <<  2) { n -=  2; i >>=  2; }
    return n - (i >> 1);
}

int Integer::numberOfTrailingZeros(int i) {
    // HD, Count trailing 0's
    i = ~i & (i - 1);
    if (i <= 0) return i & 32;
    int n = 1;
    if (i > 1 << 16) { n += 16; i >>= 16; }
    if (i > 1 <<  8) { n +=  8; i >>=  8; }
    if (i > 1 <<  4) { n +=  4; i >>=  4; }
    if (i > 1 <<  2) { n +=  2; i >>=  2; }
    return n + (i >> 1);
}

int Integer::remainderUnsigned(int dividend, int divisor) {
    return static_cast<uint>(dividend) % static_cast<uint>(divisor);
}

int Integer::reverse(int i) {
    i = (i & 0xFFFF0000) >> 16 | (i & 0x0000FFFF) << 16;
    i = (i & 0xFF00FF00) >> 8 | (i & 0x00FF00FF) << 8;
    i = (i & 0xF0F0F0F0) >> 4 | (i & 0x0F0F0F0F) << 4;
    i = (i & 0xCCCCCCCC) >> 2 | (i & 0x33333333) << 2;
    i = (i & 0xAAAAAAAA) >> 1 | (i & 0x55555555) << 1;
    return i;
}

int Integer::reverseBytes(int i) {
    return std::byteswap(i);
}

int Integer::rotateLeft(int i, int distance) {
    return (i << distance % 32) | (i >> (32 - distance) % 32);
}

int Integer::rotateRight(int i, int distance) {
    return (i >> distance % 32) | (i << (32 - distance) % 32);
}

short Integer::shortValue() {
    return static_cast<short>(boxed_value);
}

int Integer::signum(int i) {
    return i == 0 ? 0 : (i > 0 ? 1 : -1);
}

int Integer::sum(int a, int b) {
    return a + b;
}

shared<String> Integer::toString() {
    return alloc<String>(std::to_string(boxed_value));
}

shared<String> Integer::toString(int i) {
    return alloc<String>(std::to_string(i));
}

long long Integer::toUnsignedLong(int x) {
    return static_cast<uint>(x);
}

shared<String> Integer::toUnsignedString(int i) {
    return alloc<String>(std::to_string(static_cast<uint>(i)));
}

shared<Integer> Integer::valueOf(int i) {
    if (auto it = intpool.find(i); it != intpool.end())
        return it->second;
    return intpool[i] = alloc<Integer>(i);
}
