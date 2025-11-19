#include "IntStream.h"

#include <utility>

namespace java::util::stream {
    void IntStream::IntMapMultiConsumer::accept(int value, shared<function::IntConsumer> ic) {
        inner(value, std::move(ic));
    }
}
