#include "DoubleStream.h"

namespace java::util::stream {
    shared<DoubleStream::Builder> DoubleStream::Builder::add(double t) {
        this->accept(t);
        return from_self<Builder>();
    }
}
