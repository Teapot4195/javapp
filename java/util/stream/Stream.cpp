#include "Stream.h"

namespace java::util::stream {
    shared<Stream::Builder> Stream::Builder::add(const shared<Object> T) {
        accept(T);
        return std::dynamic_pointer_cast<Builder>(shared_from_this());
    }
}
