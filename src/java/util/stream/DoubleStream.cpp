#include "DoubleStream.h"

namespace java::util::stream {
    shared<DoubleStream::Builder> DoubleStream::Builder::add(double t) {
        this->accept(t);
        return std::dynamic_pointer_cast<Builder>(shared_from_this());
    }
}
